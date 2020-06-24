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
    // ���ڷ�����ͬ�¼�������/����ƶ��ȣ���ʱ�򱾺������Զ�����
    // msg �������¼������ͣ�WM_DESTROY ��ʾ���ڹر�
    // �� windows �У��¼�����֮Ϊ��Ϣ
    if (msg == WM_DESTROY) {
        PostQuitMessage(0);
        return 0;
    }
    else if (msg == WM_COMMAND) {
        // msg �� WM_COMMAND ��ʱ�򣬱����ǰ�ť����¼�
        // ��ʱ�� wp �д�ľ��ǰ�ť�� id����� id �������Լ��ڴ�����ť��ʱ���趨��
        int buttonId = (int)wp;
        buttonAction(buttonId, hWnd);
    }
    // ϵͳ��Ĭ�ϴ��ڴ�������������·
    return DefWindowProc(hWnd, msg, wp, lp);
}


int
runLoop() {
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    // ���ڽ�����Ϣѭ��
    // �������û�н��յ��˳���Ϣ�Ļ�����һֱѭ��
    while (msg.message != WM_QUIT) {
        // �����Ϣ���У������Ϣ���ڵĻ���ִ����Ϣ����
        // ���û���κ���Ϣ��GetMessage �ͻ��������
        if (GetMessage(&msg, NULL, 0, 0)) {
            // ���������ǹ涨�����ڲ������Ϊʲô
            // ������ִ�����ϵͳ�ͻ��Զ����� WndProc ����
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
        // _T ���������ַ�������������ʹ������
        auto className = _T("MainWindow");
        // ���ô������Բ���ʼ��
        WNDCLASSEX	window = { 0 };
        window.hInstance = hInstance;         // ����� WinMain �����ĵ�һ������
        window.lpfnWndProc = windowCallback;  // ϵͳ���ڴ����¼���������õĺ���
        window.lpszClassName = className;     // ���Ҫ������ CreateWindow ��һ������һ��
        // ��������ԣ����ؾ����˽⣬�����涨����
        // ʵ���������� API ��Ƶ���΢�õ�����ģ�����ܶ����Զ�Ӧ����Ĭ��ֵ�����������ֶ�ȥ����
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

        // ��������
        HWND hWnd = CreateWindow( 
            className,                  // ���Ҫ������� window �� lpszClassName ����һ��
            _T("thunder class(the teacher side)"), // ���ڱ���
            WS_OVERLAPPEDWINDOW,        // ������ʽ
            // ���� 4 �������ֱ��Ǵ������Ͻǵ� x y �����Լ����ڵ� w h
            x, y, width, height,
            // ���� 4 ������Ӧ�����涨
            NULL,
            NULL,
            hInstance,
            NULL
        );

        this->mainWindowId = hWnd;
        this->instanceId = hInstance;


        // ��ʾ�����´��ڣ������涨
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
            _T("BUTTON"),     // ��ť�����ƣ������� BUTTON
            _T("LOGIN"),   // ��ť�ı���������������
            // ��ʽ���ֱ��ǣ�����һ���Ӵ��ڣ������Ĵ��ڿɼ�������(����)
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            x, y, width, height,     // ��ť�����Ͻ������Լ����
            windowId,           // ��ť�ĸ����ڣ�Ҳ���Ƿ����ĸ�������
            (HMENU)LOGINBUTTONID,           // ��ť�� ID������ָ��
            // ���� 2 �����ڵ����涨
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
        // _T ���������ַ�������������ʹ������
        auto className = _T("LoginWindow");
        // ���ô������Բ���ʼ��
        WNDCLASSEX	window = { 0 };
        window.hInstance = mainWindow->instanceId;         // ����� WinMain �����ĵ�һ������
        window.lpfnWndProc = windowCallback;  // ϵͳ���ڴ����¼���������õĺ���
        window.lpszClassName = className;     // ���Ҫ������ CreateWindow ��һ������һ��
        // ��������ԣ����ؾ����˽⣬�����涨����
        // ʵ���������� API ��Ƶ���΢�õ�����ģ�����ܶ����Զ�Ӧ����Ĭ��ֵ�����������ֶ�ȥ����
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
            className,                  // ���Ҫ������� window �� lpszClassName ����һ��
            _T("login"), // ���ڱ���
            WS_OVERLAPPEDWINDOW,        // ������ʽ
            // ���� 4 �������ֱ��Ǵ������Ͻǵ� x y �����Լ����ڵ� w h
            x, y, width, height,
            // ���� 4 ������Ӧ�����涨
            mainWindow->mainWindowId,
            NULL,
            mainWindow->instanceId,
            NULL
        );
        this->loginWindowId = hWnd;
        this->instanceId = mainWindow->instanceId;

        // ��ʾ�����´��ڣ������涨
        ShowWindow(hWnd, SW_SHOW);
        UpdateWindow(hWnd);
    }

private:

};


class UserNameInuput {
public:
    UserNameInuput(int x, int y, int width, int height, HWND windowId, HINSTANCE hInstance) {
        CreateWindow(
            _T("Edit"),                        // ��������ƣ�����һ�����ܴ�
            _T("admin"),   // ������������Ĭ������
            // ��ʽ�趨���ֱ��ǣ��Ӵ��ڣ����ڿɼ��������������������������������б༭��
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
            _T("Edit"),                        // ��������ƣ�����һ�����ܴ�
            _T("467"),   // ������������Ĭ������
            // ��ʽ�趨���ֱ��ǣ��Ӵ��ڣ����ڿɼ��������������������������������б༭��
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
            _T("BUTTON"),     // ��ť�����ƣ������� BUTTON
            _T("VERIFY"),   // ��ť�ı���������������
            // ��ʽ���ֱ��ǣ�����һ���Ӵ��ڣ������Ĵ��ڿɼ�������(����)
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            x, y, width, height,     // ��ť�����Ͻ������Լ����
            windowId,           // ��ť�ĸ����ڣ�Ҳ���Ƿ����ĸ�������
            (HMENU)VERIFYBUTTON,           // ��ť�� ID������ָ��
            // ���� 2 �����ڵ����涨
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
            _T("BUTTON"),     // ��ť�����ƣ������� BUTTON
            _T("start classroom"),   // ��ť�ı���������������
            // ��ʽ���ֱ��ǣ�����һ���Ӵ��ڣ������Ĵ��ڿɼ�������(����)
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            x, y, width, height,     // ��ť�����Ͻ������Լ����
            mainWindow->mainWindowId,           // ��ť�ĸ����ڣ�Ҳ���Ƿ����ĸ�������
            (HMENU)STARTCLASSBUTTONID,           // ��ť�� ID������ָ��
            // ���� 2 �����ڵ����涨
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
            _T("BUTTON"),     // ��ť�����ƣ������� BUTTON
            _T("end classroom"),   // ��ť�ı���������������
            // ��ʽ���ֱ��ǣ�����һ���Ӵ��ڣ������Ĵ��ڿɼ�������(����)
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            x, y, width, height,     // ��ť�����Ͻ������Լ����
            mainWindow->mainWindowId,           // ��ť�ĸ����ڣ�Ҳ���Ƿ����ĸ�������
            (HMENU)EndClassButtonID,           // ��ť�� ID������ָ��
            // ���� 2 �����ڵ����涨
            mainWindow->instanceId,
            NULL
        );
    }

private:

};


void
buttonAction(int buttonId, HWND windowId) {

    if (buttonId == LOGINBUTTONID) {
        // ����һ������
        //MessageBox(NULL, _T("button 1 clicked"), _T("alert window 1"), MB_OK);
        LoginWindow* loginWindow = new LoginWindow(200, 200, 600, 400);
        UserNameInuput* userNameInuput = new UserNameInuput(150, 0, 300, 100, loginWindow->loginWindowId, loginWindow->instanceId);
        PasswordInuput* passwordInuput = new PasswordInuput(150, 100, 300, 100, loginWindow->loginWindowId, loginWindow->instanceId);
        VerifyButton* verifyButton = new VerifyButton(150, 200, 300, 50, loginWindow->loginWindowId, loginWindow->instanceId);
    }
    else if (buttonId == VERIFYBUTTON) {
        //MessageBox(NULL, _T("button 2 clicked"), _T("alert window 2"), MB_OK);

        // ��������� �������
        auto nameId = GetDlgItem(windowId, USERNAMEINPUTID);
        // HWND receiveEditorHwnd = GetDlgItem(windowId, EDIT_ID2);

        // ���������е�����
        // �û���
        const int nameLength = 128;
        TCHAR nameText[nameLength];
        GetWindowText(nameId, nameText, nameLength);

        CString nameContent(nameText);
        // ��Ҫע����ǣ�����㿪�����뷨����key ����һֱ�� 229
        //MessageBox(NULL, nameContent, _T("�û���"), MB_OK);

        // ����
        auto passwordId = GetDlgItem(windowId, PASSWORDINPUTID);
        //HWND receiveEditorHwnd = GetDlgItem(windowId, EDIT_ID2);

        const int passwordLength = 128;
        TCHAR passdordText[passwordLength];
        GetWindowText(passwordId, passdordText, passwordLength);

        CString passwordContent(passdordText);
        // ��Ҫע����ǣ�����㿪�����뷨����key ����һֱ�� 229
        //MessageBox(NULL, passwordContent, _T("����"), MB_OK);

        // hard code
        if (nameContent == "admin" && passwordContent == "467") {
            CString content("����ɹ�");
            MessageBox(NULL, content, _T("alert"), MB_OK);
            StartClassButton* startClassButton = new StartClassButton(400, 0, 200, 50);
            EndClassButton* endClassButton = new EndClassButton(600, 0, 200, 50);

            // turn on the server 
            // ���λ��ü���ȫ�ֱ���, ����
            server = new Server();
            // start a new thread to continuously accept student connections
            std::thread t1(Server::connectionProcess, server->socketFd, &(server->connections));
            // ò�������Ҳ��
            //std::thread t1(&Server::connectionProcess, server->socketFd, server->connections);
            // ����Ҫ����д, ��Ȼ
            // a pointer to a bound function may only be used to call the function
            // no instance of constructor matches the argument list thread

            // �ٴε��� runloop, �Է�����
            // visual c++ runtime library debug error abort() has been called
            // ���Ǻܶ�
            // ���� stack overflow, https://stackoverflow.com/questions/24855515/debug-error-abort-has-been-called �Ļش�
            // When you call superLucky from main, s is empty. stoi(s) throws an exception when s is empty.
            // The check s.size() > 10 is not robust. It is platform dependent. You can use a try/catch block to deal with it instead of hard coding a size.
            // �Լ������������ä�¿����ǿ���һ�����̺߳�, ��������(����Ӧ����ָ���߳�)ִ��ʱû�м�����������ѭ����, �����˳���, abort() has been called
            // �ÿӰ�, Ҳ��˵, �����ײ, ��Ȼ�͹�����������, �Ա�
            runLoop();
        }
        else {
            CString content("����ʧ��, �û������������");
            MessageBox(NULL, content, _T("alert"), MB_OK);
        }
    }
    else if (buttonId == STARTCLASSBUTTONID) {
        MessageBox(NULL, _T("live streaming"), _T("alert"), MB_OK);
        // change flag variable
        broadcast = true;
        // start another thread to live streaming
        std::thread t2(liveStreaming);
        // ����·��, �����ܸ� runloop
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

    // �ÿ�, �Բ�, Ҳ��д��Ҫ��ʹ�� waveInStart;
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