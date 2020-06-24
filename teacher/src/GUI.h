#pragma once


#include <tchar.h>
#include <atlstr.h>
#include <thread>
#include <sstream>


#include "windowId.h"
#include "streamDataType.h"
#include "server.h"
#include "screen.h"
#include "audio.h"


// class declare
class MainWindow;
// global variable
MainWindow* mainWindow;
Server* server;
// flag, be used to suspend streaming
bool broadcast = false;


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
            _T("thunder class(the teacher side)"), // 窗口标题
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
            _T("admin"),   // 这里是输入框的默认内容
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
            _T("467"),   // 这里是输入框的默认内容
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
    int buttonId = VERIFYBUTTON;

    VerifyButton(int x, int y, int width, int height, HWND windowId, HINSTANCE hInstance) {
        CreateWindow(
            _T("BUTTON"),     // 按钮类名称，必须是 BUTTON
            _T("VERIFY"),   // 按钮的标题名，可以随便改
            // 样式，分别是，创建一个子窗口，创建的窗口可见，按键(类型)
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            x, y, width, height,     // 按钮的左上角坐标以及宽高
            windowId,           // 按钮的父窗口，也就是放在哪个窗口里
            (HMENU)VERIFYBUTTON,           // 按钮的 ID，自行指定
            // 下面 2 个现在当做规定
            hInstance,
            NULL
        );
    }


private:

};


class StartClassButton {
public:
    int buttonId = STARTCLASSBUTTONID;
    StartClassButton(int x, int y, int width, int height) {
        CreateWindow(
            _T("BUTTON"),     // 按钮类名称，必须是 BUTTON
            _T("start classroom"),   // 按钮的标题名，可以随便改
            // 样式，分别是，创建一个子窗口，创建的窗口可见，按键(类型)
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            x, y, width, height,     // 按钮的左上角坐标以及宽高
            mainWindow->mainWindowId,           // 按钮的父窗口，也就是放在哪个窗口里
            (HMENU)STARTCLASSBUTTONID,           // 按钮的 ID，自行指定
            // 下面 2 个现在当做规定
            mainWindow->instanceId,
            NULL
        );
    }

private:

};


class EndClassButton {
public:
    int buttonId = EndClassButtonID;
    EndClassButton(int x, int y, int width, int height) {
        CreateWindow(
            _T("BUTTON"),     // 按钮类名称，必须是 BUTTON
            _T("end classroom"),   // 按钮的标题名，可以随便改
            // 样式，分别是，创建一个子窗口，创建的窗口可见，按键(类型)
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            x, y, width, height,     // 按钮的左上角坐标以及宽高
            mainWindow->mainWindowId,           // 按钮的父窗口，也就是放在哪个窗口里
            (HMENU)EndClassButtonID,           // 按钮的 ID，自行指定
            // 下面 2 个现在当做规定
            mainWindow->instanceId,
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
        LoginWindow* loginWindow = new LoginWindow(200, 200, 600, 400);
        UserNameInuput* userNameInuput = new UserNameInuput(150, 0, 300, 100, loginWindow->loginWindowId, loginWindow->instanceId);
        PasswordInuput* passwordInuput = new PasswordInuput(150, 100, 300, 100, loginWindow->loginWindowId, loginWindow->instanceId);
        VerifyButton* verifyButton = new VerifyButton(150, 200, 300, 50, loginWindow->loginWindowId, loginWindow->instanceId);
    }
    else if (buttonId == VERIFYBUTTON) {
        //MessageBox(NULL, _T("button 2 clicked"), _T("alert window 2"), MB_OK);

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
            StartClassButton* startClassButton = new StartClassButton(400, 0, 200, 50);
            EndClassButton* endClassButton = new EndClassButton(600, 0, 200, 50);

            // turn on the server 
            // 无奈还得继续全局变量, 我吐
            server = new Server();
            // start a new thread to continuously accept student connections
            std::thread t1(Server::connectionProcess, server->socketFd, &(server->connections));
            // 貌似下面的也行
            //std::thread t1(&Server::connectionProcess, server->socketFd, server->connections);
            // 上面要这样写, 不然
            // a pointer to a bound function may only be used to call the function
            // no instance of constructor matches the argument list thread

            // 再次调用 runloop, 以防报错
            // visual c++ runtime library debug error abort() has been called
            // 不是很懂
            // 根据 stack overflow, https://stackoverflow.com/questions/24855515/debug-error-abort-has-been-called 的回答
            // When you call superLucky from main, s is empty. stoi(s) throws an exception when s is empty.
            // The check s.size() > 10 is not robust. It is platform dependent. You can use a try/catch block to deal with it instead of hard coding a size.
            // 以及结合这里的情况盲猜可能是开启一个新线程后, 在跳回来(这里应该是指主线程)执行时没有继续进入无限循环体, 所以退出了, abort() has been called
            // 好坑啊, 也不说, 误打误撞, 不然就跪死在这里了, 自闭
            runLoop();
        }
        else {
            CString content("登入失败, 用户名或密码错误");
            MessageBox(NULL, content, _T("alert"), MB_OK);
        }
    }
    else if (buttonId == STARTCLASSBUTTONID) {
        MessageBox(NULL, _T("live streaming"), _T("alert"), MB_OK);
        // change flag variable
        broadcast = true;
        // start another thread to live streaming
        std::thread t2(liveStreaming);
        // 老套路了, 还得跑个 runloop
        runLoop();
    }
    else if (buttonId == EndClassButtonID)
    {
        MessageBox(NULL, _T("Live broadcast has ended"), _T("alert"), MB_OK);
    }
}


void
liveStreaming() {
    int width = 300;
    int height = 300;
    Screen* screen = new Screen(width, height);

    /*std::stringstream s;
    s << std::string("broadcast") << ": (" << broadcast << ")\n";
    CString content(s.str().c_str());
    OutputDebugString(content);*/

    // audio
    audioData = new char[audioSize];
    UINT n = waveInGetNumDevs();
    //printf("there is %d waveform-audio input devices present in the system\n", n);

    HWAVEIN phwi;

    WAVEFORMATEX pwfx;
    pwfx.wFormatTag = WAVE_FORMAT_PCM;
    pwfx.nChannels = 2;
    pwfx.nSamplesPerSec = 10000;
    pwfx.nBlockAlign = 2;
    //pwfx.nBlockAlign = pwfx.nChannels * pwfx.wBitsPerSample / 8;
    pwfx.nAvgBytesPerSec = pwfx.nSamplesPerSec * pwfx.nBlockAlign;
    pwfx.wBitsPerSample = 8;

    int waveInResult = waveInOpen(
        &phwi,
        WAVE_MAPPER,
        &pwfx,
        (DWORD_PTR)waveInProc,
        NULL,
        CALLBACK_FUNCTION
    );

    // 好坑, 卧槽, 也不写明要先使用 waveInStart;
    int inStart = waveInStart(phwi);

    WAVEHDR header;
    header.lpData = audioData + audioOffset + 5;
    header.dwBufferLength = 10000;
    header.dwBytesRecorded = header.dwBufferLength;
    header.dwUser = NULL;
    header.dwFlags = 0;
    int waveInPrepare = waveInPrepareHeader(phwi, &header, sizeof(WAVEHDR));
    int waveInAddBuff = waveInAddBuffer(
        phwi,
        &header,
        sizeof(WAVEHDR)
    );
    audioOffset += audioBufferSize;
    if (audioOffset >= audioSize) {
        audioOffset = 0;
    }

    // write audio data header
    int samplePerSec = 10000;
    *(audioData + audioBufferSize + 0) = samplePerSec >> 0;
    *(audioData + audioBufferSize + 1) = samplePerSec >> 8;
    *(audioData + audioBufferSize + 2) = samplePerSec >> 16;
    *(audioData + audioBufferSize + 3) = samplePerSec >> 24;
    *(audioData + audioBufferSize + 4) = AUDIOTYPE;

    *(audioData + 0) = samplePerSec >> 0;
    *(audioData + 1) = samplePerSec >> 8;
    *(audioData + 2) = samplePerSec >> 16;
    *(audioData + 3) = samplePerSec >> 24;
    *(audioData + 4) = AUDIOTYPE;

    while (true) {
        if (broadcast) {
            // get screen pixels data
            const char* pixels = screen->capture();
            const int pixelsSize = screen->size();

            // point compressed pixels data, screen->pixelsCompress will init it;
            char* compressedPixels;
            // compressed pixels size, include header, screen->pixelsCompress will overwrite it;
            int compressedPixelsSize = 0;

            screen->pixelsCompress((const unsigned char*)pixels, pixelsSize, &compressedPixels, (size_t*)&compressedPixelsSize);

            std::stringstream s;
            s << std::string("connections size") << ": (" << server->connections.size() << ")\n";
            CString content(s.str().c_str());
            OutputDebugString(content);

            for (int i = 0; i < server->connections.size(); i++) {

                server->sendPixels(server->connections[i], compressedPixels, compressedPixelsSize);
                // right here, audioOffset like semaphore
                if (audioOffset == 0) {
                    //log("audioOffset = 0");
                    server->sendAudio(server->connections[i], audioData + audioBufferSize, audioBufferSize);
                }
                else if (audioOffset == audioBufferSize) {
                    //log("audioOffset = 10005");
                    server->sendAudio(server->connections[i], audioData, audioBufferSize);
                }

            }

            // delete pixels data
            delete[] pixels;
            // delete compressed pixels data
            delete[] compressedPixels;
        }

        // sleep
        std::this_thread::sleep_for(std::chrono::milliseconds(300));

    }
}