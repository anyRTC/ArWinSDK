# Open Video Call Windows

*English | [中文](README.zh.md)*

The Open Video Call Windows Sample App is an open-source demo that will help you get video chat integrated directly into your Windows applications using the AR4.0 Video SDK.

With this sample app, you can:

- Join / leave channel
- Mute / unmute audio
- Enable / disable video
- Change camera
- Send message to channel
- Setup resolution, frame rate and bit rate
- Enable encryption
- Enable / disable black and white filter

This demo is written in **C++**

A tutorial demo can be found here: [AR4.0-Windows-Tutorial-1to1]()

AR4.0 Video SDK supports iOS / Android / Windows / macOS etc. You can find demos of these platform here:

- [OpenVideoCall-Android](https://github.com/anyRTC/ArAndroidSDK)
- [OpenVideoCall-iOS](https://github.com/anyRTC/AriOSSDK)

## Prerequisites

- Visual Studio 2013 or higher
- Windows 7 or higher

## Quick Start

This section shows you how to prepare, build, and run the sample application.

### Obtain an App ID

To build and run the sample application, get an App ID:
1. Create a developer account at [anyrtc.io](https://console.anyrtc.io/signin/). Once you finish the signup process, you will be redirected to the Dashboard.
2. Navigate in the Dashboard tree on the left to **Projects** > **Project List**.
3. Save the **App ID** from the Dashboard for later use.
4. Generate a temp **Access Token** (valid for 24 hours) from dashboard page with given channel name, save for later use.

4. Define the APP_ID with your App ID.

    ```
    #define APP_ID _T("Your App ID")
    ```

### Integrate the AR4.0 Video SDK

1. Download the **AR4.0 Video SDK** from [anyRTC.io SDK](https://www.anyrtc.io/blog/download/). Unzip the downloaded SDK package and copy the **sdk** to the `OpenVideoCall` folder in project（the old one may be over written.
2. Open OpenVideoCall.sln, build the solution and run.


### Note

1. After the program is compiled, if the program "xxx\xxx\xxx\Debug\Language\English.dll" cannot be started when running the program, 

please select the OpenVideoCall project in the Solution Explorer and right click. In the pop-up menu bar, select "Set as active project" to solve. Then run the program again.

2. The dll library under the sdk/dll file needs to be placed in the corresponding execution path.

Tips: The relevant dll library has been configured for you in this case tutorial. If you want to use the interface provided by anyrtc for related development, you need to put the dll library into the corresponding execution path as prompted above.

## Resources

- You can find full API document at [Document Center](https://docs.anyrtc.io)
- You can file bugs about this demo at [issue](https://github.com/anyRTC/ArWinSDK/issues)

## License

The MIT License (MIT)
