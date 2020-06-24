#pragma once


#include <tchar.h>
#include <atlstr.h>
#include <thread>
#include <sstream>

#include "windowId.h"
#include "client.h"
#include "screen.h"
#include "audio.h"


// class declare
class MainWindow;
// global variable
MainWindow* mainWindow;
Client* client;


// function declare
void
buttonAction(int buttonId, HWND windowId);

void 
liveStreaming();


LRESULT CALLBACK
windowCallback(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
    // 窗口发生不同事件（案件/鼠标移动等）的时候本函数会自动调用
    // msg 代表了事件的类型，WM_DESTROY 表示窗口关闭
    // 在 windows 中，事件被称之为消息
    if (msg == WM_DESTROY) {
        PostQuitMessage(0);
        return 0;
    }
    else if (msg == WM_COMMAND) {
        // msg 是 WM_COMMAND 的时候，表明是按钮点击事件
        // 这时候 wp 中存的就是按钮的 id，这个 id 是我们自己在创建按钮的时候设定的
        int buttonId = (int)wp;
        buttonAction(buttonId, hWnd);
    }
    // 系统的默认窗口处理函数，这是套路
    return DefWindowProc(hWnd, msg, wp, lp);
}


int
runLoop() {
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    // 窗口进入消息循环
    // 如果窗口没有接收到退出消息的话，就一直循环
    while (msg.message != WM_QUIT) {
        // 检查消息队列，如果消息存在的话，执行消息处理
        // 如果没有任何消息，GetMessage 就会等在这里
        if (GetMessage(&msg, NULL, 0, 0)) {
            // 下面两行是规定，现在不必理解为什么
            // 这两行执行完后，系统就会自动调用 WndProc 函数
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return (int)msg.wParam;
}


class MainWindow {
public:
    HWND mainWindowId;
    HINSTANCE instanceId;

	MainWindow(HINSTANCE hInstance, int x, int y, int width, int height) {
        // _T 包起来的字符串才能在里面使用中文
        auto className = _T("MainWindow");
        // 设置窗口属性并初始化
        WNDCLASSEX	window = { 0 };
        window.hInstance = hInstance;         // 这个是 WinMain 函数的第一个参数
        window.lpfnWndProc = windowCallback;  // 系统会在窗口事件发生后调用的函数
        window.lpszClassName = className;     // 这个要和下面 CreateWindow 第一个参数一样
        // 下面的属性，不必具体了解，当做规定即可
        // 实际上如果这个 API 设计得稍微用点点点点心，下面很多属性都应该有默认值而不是让人手动去设置
        window.cbSize = sizeof(WNDCLASSEX);
        window.cbClsExtra = 0;
        window.cbWndExtra = 0;
        window.style = CS_CLASSDC;
        window.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        window.hCursor = LoadCursor(NULL, IDC_ARROW);
        window.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        window.lpszMenuName = NULL;
        window.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
        RegisterClassEx(&window);

        // 创建窗口
        HWND hWnd = CreateWindow( 
            className,                  // 这个要和上面的 window 的 lpszClassName 属性一样
            _T("thunder class(the student side)"), // 窗口标题
            WS_OVERLAPPEDWINDOW,        // 窗口样式
            // 下面 4 个参数分别是窗口左上角的 x y 坐标以及窗口的 w h
            x, y, width, height,
            // 下面 4 个现在应当做规定
            NULL,
            NULL,
            hInstance,
            NULL
        );

        this->mainWindowId = hWnd;
        this->instanceId = hInstance;


        // 显示，更新窗口，当做规定
        ShowWindow(hWnd, SW_SHOW);
        UpdateWindow(hWnd);
	}


private:

};


class LoginButton {
public:
    int buttonId = LOGINBUTTONID;

    LoginButton(int x, int y, int width, int height, HWND windowId, HINSTANCE hInstance) {
        CreateWindow(
            _T("BUTTON"),     // 按钮类名称，必须是 BUTTON
            _T("LOGIN"),   // 按钮的标题名，可以随便改
            // 样式，分别是，创建一个子窗口，创建的窗口可见，按键(类型)
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            x, y, width, height,     // 按钮的左上角坐标以及宽高
            windowId,           // 按钮的父窗口，也就是放在哪个窗口里
            (HMENU)LOGINBUTTONID,           // 按钮的 ID，自行指定
            // 下面 2 个现在当做规定
            hInstance,
            NULL
        );
    }


private:

};


class LoginWindow {
public:
    HWND loginWindowId;
    HINSTANCE instanceId;

    LoginWindow(int x, int y, int width, int height) {
        // _T 包起来的字符串才能在里面使用中文
        auto className = _T("LoginWindow");
        // 设置窗口属性并初始化
        WNDCLASSEX	window = { 0 };
        window.hInstance = mainWindow->instanceId;         // 这个是 WinMain 函数的第一个参数
        window.lpfnWndProc = windowCallback;  // 系统会在窗口事件发生后调用的函数
        window.lpszClassName = className;     // 这个要和下面 CreateWindow 第一个参数一样
        // 下面的属性，不必具体了解，当做规定即可
        // 实际上如果这个 API 设计得稍微用点点点点心，下面很多属性都应该有默认值而不是让人手动去设置
        window.cbSize = sizeof(WNDCLASSEX);
        window.cbClsExtra = 0;
        window.cbWndExtra = 0;
        window.style = CS_CLASSDC;
        window.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        window.hCursor = LoadCursor(NULL, IDC_ARROW);
        window.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        window.lpszMenuName = NULL;
        window.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
        RegisterClassEx(&window);

        HWND hWnd = CreateWindow(
            className,                  // 这个要和上面的 window 的 lpszClassName 属性一样
            _T("login"), // 窗口标题
            WS_OVERLAPPEDWINDOW,        // 窗口样式
            // 下面 4 个参数分别是窗口左上角的 x y 坐标以及窗口的 w h
            x, y, width, height,
            // 下面 4 个现在应当做规定
            mainWindow->mainWindowId,
            NULL,
            mainWindow->instanceId,
            NULL
        );
        this->loginWindowId = hWnd;
        this->instanceId = mainWindow->instanceId;

        // 显示，更新窗口，当做规定
        ShowWindow(hWnd, SW_SHOW);
        UpdateWindow(hWnd);
    }

private:

};


class UserNameInuput {
public:
    UserNameInuput(int x, int y, int width, int height, HWND windowId, HINSTANCE hInstance) {
        CreateWindow(
            _T("Edit"),                        // 输入框名称，这里一定不能错
            _T("username"),   // 这里是输入框的默认内容
            // 样式设定，分别是，子窗口，窗口可见，带纵向滚动条，带横向滚动条，多行编辑框，
            WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE,
            x, y, width, height,
            windowId,
            (HMENU)USERNAMEINPUTID,
            hInstance,
            NULL
        );
    }

private:

};


class PasswordInuput {
public:
    PasswordInuput(int x, int y, int width, int height, HWND windowId, HINSTANCE hInstance) {
        CreateWindow(
            _T("Edit"),                        // 输入框名称，这里一定不能错
            _T("password"),   // 这里是输入框的默认内容
            // 样式设定，分别是，子窗口，窗口可见，带纵向滚动条，带横向滚动条，多行编辑框，
            WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE,
            x, y, width, height,
            windowId,
            (HMENU)PASSWORDINPUTID,
            hInstance,
            NULL
        );
    }

private:

};


class VerifyButton {
public:
    int buttonId = VERIFYBUTTONID;

    VerifyButton(int x, int y, int width, int height, HWND windowId, HINSTANCE hInstance) {
        CreateWindow(
            _T("BUTTON"),     // 按钮类名称，必须是 BUTTON
            _T("VERIFY"),   // 按钮的标题名，可以随便改
            // 样式，分别是，创建一个子窗口，创建的窗口可见，按键(类型)
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            x, y, width, height,     // 按钮的左上角坐标以及宽高
            windowId,           // 按钮的父窗口，也就是放在哪个窗口里
            (HMENU)VERIFYBUTTONID,           // 按钮的 ID，自行指定
            // 下面 2 个现在当做规定
            hInstance,
            NULL
        );
    }


private:

};


void
buttonAction(int buttonId, HWND windowId) {

    if (buttonId == LOGINBUTTONID) {
        // 创建一个弹窗
        //MessageBox(NULL, _T("button 1 clicked"), _T("alert window 1"), MB_OK);

        // 暂时不登入, 直接连接服务器
        //LoginWindow* loginWindow = new LoginWindow(200, 200, 600, 400);
        //UserNameInuput* userNameInuput = new UserNameInuput(150, 0, 300, 100, loginWindow->loginWindowId, loginWindow->instanceId);
        //PasswordInuput* passwordInuput = new PasswordInuput(150, 100, 300, 100, loginWindow->loginWindowId, loginWindow->instanceId);
        //VerifyButton* verifyButton = new VerifyButton(150, 200, 300, 50, loginWindow->loginWindowId, loginWindow->instanceId);

        // 逃不了全局变量的噩梦
        client = new Client();
        if (client->connectionResult == 0) {
            client->login();
            if (client->loginResult()) {
                MessageBox(NULL, _T("登入成功, 准备上课了哦"), _T("alert"), MB_OK);
                std::thread t1(liveStreaming);
                //liveStreaming();
                // 老规矩 再来 runloop, 应该是把程序控制权在交回 runLoop
                runLoop();
            }
            else {
                MessageBox(NULL, _T("登入失败"), _T("alert"), MB_OK);
            }
        }
        else {
            MessageBox(NULL, _T("连接服务器失败, 或者老师还未上线"), _T("alert"), MB_OK);
        }
    }
    else if (buttonId == VERIFYBUTTONID) {
        //MessageBox(NULL, _T("button 2 clicked"), _T("alert window 2"), MB_OK);

        // 这里目前不会有, 不会进入这里

        // 获得输入框的 “句柄”
        auto nameId = GetDlgItem(windowId, USERNAMEINPUTID);
        // HWND receiveEditorHwnd = GetDlgItem(windowId, EDIT_ID2);

        // 获得输入框中的内容
        // 用户名
        const int nameLength = 128;
        TCHAR nameText[nameLength];
        GetWindowText(nameId, nameText, nameLength);

        CString nameContent(nameText);
        // 需要注意的是，如果你开了输入法啊，key 将会一直是 229
        //MessageBox(NULL, nameContent, _T("用户名"), MB_OK);

        // 密码
        auto passwordId = GetDlgItem(windowId, PASSWORDINPUTID);
        //HWND receiveEditorHwnd = GetDlgItem(windowId, EDIT_ID2);

        const int passwordLength = 128;
        TCHAR passdordText[passwordLength];
        GetWindowText(passwordId, passdordText, passwordLength);

        CString passwordContent(passdordText);
        // 需要注意的是，如果你开了输入法啊，key 将会一直是 229
        //MessageBox(NULL, passwordContent, _T("密码"), MB_OK);

        // hard code
        if (nameContent == "admin" && passwordContent == "467") {
            CString content("登入成功");
            MessageBox(NULL, content, _T("alert"), MB_OK);
        }
        else {
            CString content("登入失败, 用户名或密码错误");
            MessageBox(NULL, content, _T("alert"), MB_OK);
        }
    }
}


void liveStreaming() {
    //Screen* screen = new Screen();

    // audio
    audioData = new char[audioSize];
    HWAVEOUT phwo;

    WAVEFORMATEX pwfx;
    pwfx.wFormatTag = WAVE_FORMAT_PCM;
    pwfx.nChannels = 2;
    pwfx.nSamplesPerSec = 10000;
    pwfx.nBlockAlign = 2;
    // pwfx.nBlockAlign = (pwfx.nChannels * pwfx.wBitsPerSample) / 8;
    pwfx.wBitsPerSample = 8;
    pwfx.nAvgBytesPerSec = pwfx.nSamplesPerSec * pwfx.nBlockAlign;

    int open = waveOutOpen(
        &phwo,
        WAVE_MAPPER,
        &pwfx,
        (DWORD_PTR)waveOutProc,
        NULL,
        CALLBACK_FUNCTION
    );

    WAVEHDR header;
    header.lpData = audioData + audioOffset;
    if (audioOffset >= audioSize) {
        audioOffset = 0;
    }
    audioOffset += audioBufferSize;
    header.dwBufferLength = audioBufferSize;
    header.dwUser = NULL;
    header.dwLoops = 0;
    header.dwFlags = 0;

    hasAudioData = true;

    while (true) {
        int size;
        int type;
        char* buff = Client::readStreamData(client->socketFd, size, type);
        std::stringstream s;
        s << std::string("debug buff size") << ": (" << size << ")\n";
        CString content(s.str().c_str());
        OutputDebugString(content);
        if (type == PIXELTYPE) {
            //uLongf outSize = screen->size();
            //char* data = (char*)malloc(outSize);
            // 用 new 申请 size 字节的内存
            //char* data = new char[outSize];
            //log("uncompress size %d", size);
            char *data = Screen::pixelsUncompress((Bytef*)buff, size);
            Screen::draw(data, mainWindow->mainWindowId);
            free(data);
            //delete[] data;
            //delete[] buff;
        }
        else if (type == AUDIOTYPE) {
            audioData = buff;

            // the first time to write audio data, trigger waveOutPro callback
            if (hasAudioData) {
                waveOutPrepareHeader(phwo, &header, sizeof(WAVEHDR));
                waveOutWrite(
                    phwo,
                    &header,
                    sizeof(WAVEHDR)
                );
                hasAudioData = false;
            }
        }

        // sleep
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
}