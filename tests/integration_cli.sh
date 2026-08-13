#!/bin/sh
# 端到端集成测试:通过 PATH 注入 mock adb/mvt-android,
# 验证 CLI 子命令、-s 传递、manifest/custody 生成与 verify 检测能力。
# 用法: integration_cli.sh <phone_forensic可执行文件> <工作目录>
set -u

BIN="$1"
WORK="$2"
if [ -z "$BIN" ] || [ -z "$WORK" ]; then
    echo "用法: integration_cli.sh <phone_forensic可执行文件> <工作目录>" >&2
    exit 1
fi

rm -rf "$WORK"
mkdir -p "$WORK"
export LQZ_ADB_LOG="$WORK/adb.log"

fail() { echo "[FAIL] $1" >&2; exit 1; }
step() { echo "[....] $1"; }

# 1. dump-sms 带序列号:命令应带 -s TEST123,并生成 manifest/custody
step "dump-sms -s TEST123"
out="$WORK/out_sms"
"$BIN" dump-sms -s TEST123 -o "$out" >/dev/null 2>&1 || fail "dump-sms 退出码非 0"
grep -q -- "-s TEST123" "$LQZ_ADB_LOG" || fail "adb 日志缺少 -s TEST123"
[ -f "$out/manifest.json" ] || fail "manifest.json 不存在"
[ -f "$out/custody.log" ] || fail "custody.log 不存在"
[ -f "$out/mvt_report.txt" ] || fail "提取产物 mvt_report.txt 不存在"
grep -q '"sha256":' "$out/manifest.json" || fail "manifest 缺少 sha256 记录"

# 2. verify --deep 应通过
step "verify --deep"
"$BIN" verify --deep -o "$out" >/dev/null 2>&1 || fail "verify --deep 未通过"

# 3. 篡改 custody 后 verify 应失败
step "篡改 custody.log 检测"
echo '{"tampered":true}' >> "$out/custody.log"
if "$BIN" verify -o "$out" >/dev/null 2>&1; then
    fail "篡改未被检出"
fi

# 4. 设备离线 → 退出码 3
step "设备离线退出码 3"
LQZ_MOCK_OFFLINE=1 "$BIN" dump-photos -s X -o "$WORK/off" >/dev/null 2>&1
rc=$?
[ "$rc" -eq 3 ] || fail "离线退出码应为 3,实际 $rc"

# 5. 参数错误 → 退出码 2
step "参数错误退出码 2"
"$BIN" dump-sms --bogus >/dev/null 2>&1
rc=$?
[ "$rc" -eq 2 ] || fail "--bogus 退出码应为 2,实际 $rc"
"$BIN" no-such-command >/dev/null 2>&1
rc=$?
[ "$rc" -eq 2 ] || fail "未知子命令退出码应为 2,实际 $rc"

# 6. help/version → 0
step "help/version"
"$BIN" --help >/dev/null 2>&1 || fail "--help 应成功"
"$BIN" --version >/dev/null 2>&1 || fail "--version 应成功"

# 7. --json 输出:stdout 恰一行且含 ok 状态与 manifest 摘要
step "--json 输出"
json_out="$("$BIN" dump-photos -s TEST123 -o "$WORK/out_json" --json)"
line_count=$(printf '%s\n' "$json_out" | wc -l | tr -d ' ')
[ "$line_count" -eq 1 ] || fail "--json 应为单行,实际 $line_count 行"
printf '%s' "$json_out" | grep -q '"status":"ok"' || fail "--json 应含 ok 状态"
printf '%s' "$json_out" | grep -q '"manifest_sha256":"' || fail "--json 应含 manifest_sha256"

step "全部集成测试通过"
exit 0
