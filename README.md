<div align="center">
    <img src="assets/logo.svg" width="200" alt="灵取证 Logo"/>
    <h1>灵取证</h1>
    <p>专业的Android设备数据取证工具</p>
  <p>
    <img src="https://img.shields.io/badge/版本-1.0.0-blue.svg" alt="version"/>
    <img src="https://img.shields.io/badge/许可证-Mulan PSL-green.svg" alt="license"/>
    <img src="https://img.shields.io/badge/平台-MacOS%20%7C%20Linux | Windows-White.svg" alt="platform"/>
    <img src="https://img.shields.io/badge/语言-C++-red.svg" alt="language"/>
    
  </p>

</div>

## 简介

灵取证是一款专业的Android设备数据取证工具，专门设计用于执法部门进行合法的数据提取和分析。本工具支持全面的数据获取功能，包括通话记录、短信、照片、视频等多种数据类型的提取，并严格遵守相关法律法规。

## 法律依据与合规性

本工具的开发和使用严格遵循以下法律法规：

- 《中华人民共和国刑事诉讼法》第 138 条

  > 规定了电子数据的收集、固定、审查、判断程序

  - 电子数据是法定证据种类之一
  - 收集电子数据应当严格遵守法定程序
  - 必须确保数据的完整性、真实性和合法性
  - 违反程序可能导致证据无效
  - 非法获取数据将承担法律责任

- 《中华人民共和国网络安全法》第 50 条

  > 规定了网络运营者配合网络安全监督管理

  - 必须在法律授权范围内进行数据获取
  - 禁止未经授权访问他人数据
  - 违反规定最高可处罚金 100 万元
  - 构成犯罪的将追究刑事责任
  - 可能导致运营许可证被吊销

- 《电子数据取证规范》GA/T 1070-2013
  > 规定了电子数据取证的标准操作规程
  - 取证过程必须遵循无损性原则
  - 要求记录完整的取证过程
  - 必须保证数据的同一性
  - 需建立完整的证据链
  - 取证人员应具备相应资质

### 法律后果警示

违反上述法律法规可能导致：

- 刑事处罚：构成犯罪的，处 3 年以下有期徒刑或者拘役
- 行政处罚：最高可处 100 万元罚款
- 民事责任：赔偿受害方经济损失
- 职业风险：可能被吊销相关执业资格

## 免责声明

- 本工具仅供执法部门在法律框架内使用
- 作者对任何非法使用或滥用概不负责
- 使用本工具即表示同意遵守相关法律法规

## 使用演示

![灵取证使用演示](assets/demo.png)

## 系统要求

### 硬件要求
- CPU: 双核处理器及以上
- 内存: 4GB RAM 及以上
- 硬盘空间: 最少 1GB 可用空间
- USB端口: USB 2.0 或以上

### 软件要求

| 操作系统 | 最低版本要求 
|---------|------------|
| Windows | Windows 7 64位 | 
| macOS | macOS 10.14 | 
| Linux | Ubuntu 18.04 LTS | 

## 安装步骤

## 环境要求

### 必需组件
1. C++ 开发环境
   - GCC/G++ 7.0 或更高版本
   - CMake 3.10 或更高版本
   - Make 工具

2. 数据库支持
   - SQLite 3.0 或更高版本

3. 版本控制
   - Git 2.0 或更高版本

4. Android 开发工具
   - Android SDK Platform Tools
   - ADB (Android Debug Bridge)

### Windows 安装
1. 安装 ADB 工具
```powershell
winget install Google.PlatformTools
````

2. 安装依赖项

```powershell
winget install Python.Python.3.8
```

### macOS 安装

1. 安装 Homebrew（如未安装）

```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

2. 安装 ADB 工具

```bash
brew install android-platform-tools
```

### Linux 安装

```bash
sudo apt update
sudo apt install adb android-tools-adb android-tools-fastboot
```

## 功能特性

| 功能          | 描述                                         | 命令 |
| ------------- | -------------------------------------------- | ---- |
| 通话记录导出  | 提取设备所有通话记录，包括时间、号码、时长等 | `1`  |
| 短信导出      | 导出设备中的短信记录，包括收发时间、内容等   | `2`  |
| 照片导出      | 提取设备中的照片文件                         | `3`  |
| 视频导出      | 提取设备中的视频文件                         | `4`  |
| 应用导出      | 导出已安装的应用程序 APK 文件                | `5`  |
| 文档导出      | 提取设备中的文档文件                         | `6`  |
| 敏感信息提取  | 提取设备中的敏感数据信息                     | `7`  |
| 无线连接      | 通过 WiFi 连接设备                           | `8`  |
| WiFi 信息     | 获取设备的 WiFi 连接信息                     | `9`  |
| 系统信息      | 导出设备系统信息                             | `10` |
| 应用列表      | 获取已安装应用列表                           | `11` |
| WhatsApp 数据 | 提取 WhatsApp 聊天记录和媒体文件             | `12` |
| 浏览器数据    | 提取浏览器历史记录和缓存                     | `13` |
| 通知记录      | 导出设备通知历史                             | `14` |


## 注意事项

1. 使用前请确保设备已开启 USB 调试模式
2. 首次连接设备时需要在设备上确认授权
3. 建议使用原装 USB 线缆连接设备
4. 数据提取过程中请勿断开设备连接
5. 定期备份提取的数据

## 许可证

本项目采用 **木兰宽松许可证 (Mulan PSL)** 进行许可。  
有关详细信息，请参阅 [LICENSE](LICENSE) 文件。

[![License: Mulan PSL v2](https://img.shields.io/badge/License-Mulan%20PSL%202-blue.svg)](http://license.coscl.org.cn/MulanPSL2)

## 🌟 开源项目赞助计划

### 用捐赠助力发展

感谢您使用本项目！您的支持是开源持续发展的核心动力。  
每一份捐赠都将直接用于：  
✅ 服务器与基础设施维护  
✅ 新功能开发与版本迭代  
✅ 文档优化与社区建设

点滴支持皆能汇聚成海，让我们共同打造更强大的开源工具！

---

### 🌐 全球捐赠通道

#### 国内用户

<div align="center" style="margin: 40px 0">

<div align="center">
<table>
<tr>
<td align="center" width="300">
<img src="https://github.com/ctkqiang/ctkqiang/blob/main/assets/IMG_9863.jpg?raw=true" width="200" />
<br />
<strong>🔵 支付宝</strong>
</td>
<td align="center" width="300">
<img src="https://github.com/ctkqiang/ctkqiang/blob/main/assets/IMG_9859.JPG?raw=true" width="200" />
<br />
<strong>🟢 微信支付</strong>
</td>
</tr>
</table>
</div>
</div>

#### 国际用户

<div align="center" style="margin: 40px 0">
  <a href="https://qr.alipay.com/fkx19369scgxdrkv8mxso92" target="_blank">
    <img src="https://img.shields.io/badge/Alipay-全球支付-00A1E9?style=flat-square&logo=alipay&logoColor=white&labelColor=008CD7">
  </a>
  
  <a href="https://ko-fi.com/F1F5VCZJU" target="_blank">
    <img src="https://img.shields.io/badge/Ko--fi-买杯咖啡-FF5E5B?style=flat-square&logo=ko-fi&logoColor=white">
  </a>
  
  <a href="https://www.paypal.com/paypalme/ctkqiang" target="_blank">
    <img src="https://img.shields.io/badge/PayPal-安全支付-00457C?style=flat-square&logo=paypal&logoColor=white">
  </a>
  
  <a href="https://donate.stripe.com/00gg2nefu6TK1LqeUY" target="_blank">
    <img src="https://img.shields.io/badge/Stripe-企业级支付-626CD9?style=flat-square&logo=stripe&logoColor=white">
  </a>
</div>

---

### 📌 开发者社交图谱

#### 技术交流

<div align="center" style="margin: 20px 0">
  <a href="https://github.com/ctkqiang" target="_blank">
    <img src="https://img.shields.io/badge/GitHub-开源仓库-181717?style=for-the-badge&logo=github">
  </a>
  
  <a href="https://stackoverflow.com/users/10758321/%e9%92%9f%e6%99%ba%e5%bc%ba" target="_blank">
    <img src="https://img.shields.io/badge/Stack_Overflow-技术问答-F58025?style=for-the-badge&logo=stackoverflow">
  </a>
  
  <a href="https://www.linkedin.com/in/ctkqiang/" target="_blank">
    <img src="https://img.shields.io/badge/LinkedIn-职业网络-0A66C2?style=for-the-badge&logo=linkedin">
  </a>
</div>

#### 社交互动

<div align="center" style="margin: 20px 0">
  <a href="https://www.instagram.com/ctkqiang" target="_blank">
    <img src="https://img.shields.io/badge/Instagram-生活瞬间-E4405F?style=for-the-badge&logo=instagram">
  </a>
  
  <a href="https://twitch.tv/ctkqiang" target="_blank">
    <img src="https://img.shields.io/badge/Twitch-技术直播-9146FF?style=for-the-badge&logo=twitch">
  </a>
  
  <a href="https://github.com/ctkqiang/ctkqiang/blob/main/assets/IMG_9245.JPG?raw=true" target="_blank">
    <img src="https://img.shields.io/badge/微信公众号-钟智强-07C160?style=for-the-badge&logo=wechat">
  </a>
</div>

---

🙌 感谢您成为开源社区的重要一员！  
💬 捐赠后欢迎通过社交平台与我联系，您的名字将出现在项目致谢列表！
