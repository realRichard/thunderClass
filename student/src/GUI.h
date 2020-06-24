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
            _T("thunder class(the student side)"), // ���ڱ���
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
            _T("username"),   // ������������Ĭ������
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
            _T("password"),   // ������������Ĭ������
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
    int buttonId = VERIFYBUTTONID;

    VerifyButton(int x, int y, int width, int height, HWND windowId, HINSTANCE hInstance) {
        CreateWindow(
            _T("BUTTON"),     // ��ť�����ƣ������� BUTTON
            _T("VERIFY"),   // ��ť�ı���������������
            // ��ʽ���ֱ��ǣ�����һ���Ӵ��ڣ������Ĵ��ڿɼ�������(����)
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            x, y, width, height,     // ��ť�����Ͻ������Լ����
            windowId,           // ��ť�ĸ����ڣ�Ҳ���Ƿ����ĸ�������
            (HMENU)VERIFYBUTTONID,           // ��ť�� ID������ָ��
            // ���� 2 �����ڵ����涨
            hInstance,
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

        // ��ʱ������, ֱ�����ӷ�����
        //LoginWindow* loginWindow = new LoginWindow(200, 200, 600, 400);
        //UserNameInuput* userNameInuput = new UserNameInuput(150, 0, 300, 100, loginWindow->loginWindowId, loginWindow->instanceId);
        //PasswordInuput* passwordInuput = new PasswordInuput(150, 100, 300, 100, loginWindow->loginWindowId, loginWindow->instanceId);
        //VerifyButton* verifyButton = new VerifyButton(150, 200, 300, 50, loginWindow->loginWindowId, loginWindow->instanceId);

        // �Ӳ���ȫ�ֱ�����ج��
        client = new Client();
        if (client->connectionResult == 0) {
            client->login();
            if (client->loginResult()) {
                MessageBox(NULL, _T("����ɹ�, ׼���Ͽ���Ŷ"), _T("alert"), MB_OK);
                std::thread t1(liveStreaming);
                //liveStreaming();
                // �Ϲ�� ���� runloop, Ӧ���ǰѳ������Ȩ�ڽ��� runLoop
                runLoop();
            }
            else {
                MessageBox(NULL, _T("����ʧ��"), _T("alert"), MB_OK);
            }
        }
        else {
            MessageBox(NULL, _T("���ӷ�����ʧ��, ������ʦ��δ����"), _T("alert"), MB_OK);
        }
    }
    else if (buttonId == VERIFYBUTTONID) {
        //MessageBox(NULL, _T("button 2 clicked"), _T("alert window 2"), MB_OK);

        // ����Ŀǰ������, �����������

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
        }
        else {
            CString content("����ʧ��, �û������������");
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
            // �� new ���� size �ֽڵ��ڴ�
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