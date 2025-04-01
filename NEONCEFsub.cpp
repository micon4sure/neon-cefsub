#include "include/cef_app.h"
#include "include/cef_command_line.h"
#include "include/wrapper/cef_message_router.h"
#include <windows.h>
#include <dxgi.h>
#include <d3d11.h> // For Direct3D 11
#include <wrl.h>   // For Microsoft::WRL::ComPtr
#include <string>
#include <fstream>

#pragma comment(lib, "dxgi.lib")  // Link DXGI library
#pragma comment(lib, "d3d11.lib") // Link D3D11 library

// void LogToFile(const std::string &message)
// {
//     std::ofstream log_file("D:\\0_WORKBENCH\\_archive\\NEON_dev_d3dtest\\cef-sub.log", std::ios_base::app);
//     if (log_file.is_open())
//     {
//         log_file << message << std::endl;
//     }
// }

class NEONRenderProcessHandler : public CefRenderProcessHandler
{
public:
    NEONRenderProcessHandler()
    {
        // LogToFile("I] NEONRenderProcessHandler constructor");
        CefMessageRouterConfig config;
        mMessageRouter = CefMessageRouterRendererSide::Create(config);
    }

    void OnContextCreated(CefRefPtr<CefBrowser> browser,
                          CefRefPtr<CefFrame> frame,
                          CefRefPtr<CefV8Context> context) override
    {
        // LogToFile("I] NEONRenderProcessHandler::OnContextCreated");
        mMessageRouter->OnContextCreated(browser, frame, context);
    }

    void OnContextReleased(CefRefPtr<CefBrowser> browser,
                           CefRefPtr<CefFrame> frame,
                           CefRefPtr<CefV8Context> context) override
    {
        // LogToFile("I] NEONRenderProcessHandler::OnContextReleased");
        mMessageRouter->OnContextReleased(browser, frame, context);
    }

    bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                  CefRefPtr<CefFrame> frame,
                                  CefProcessId source_process,
                                  CefRefPtr<CefProcessMessage> message) override
    {
        // LogToFile("I] NEONRenderProcessHandler::OnProcessMessageReceived");
        mMessageRouter->OnProcessMessageReceived(browser, frame, source_process, message);
        return 1;
    }

private:
    CefRefPtr<CefMessageRouterRendererSide> mMessageRouter;

    IMPLEMENT_REFCOUNTING(NEONRenderProcessHandler);
};

class NEONCEFsubApp : public CefApp
{
public:
    NEONCEFsubApp()
    {
        // LogToFile("I] NEONCEFsubApp constructor");
        mRenderProcessHandler = new NEONRenderProcessHandler();
    }

    CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() override
    {
        return mRenderProcessHandler;
    }

private:
    CefRefPtr<NEONRenderProcessHandler> mRenderProcessHandler;

    IMPLEMENT_REFCOUNTING(NEONCEFsubApp);
};

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // Provide CEF with command-line arguments.
    CefMainArgs main_args(hInstance);

    // Parse command-line arguments using CEF's utility.
    CefRefPtr<CefCommandLine> command_line = CefCommandLine::CreateCommandLine();
    command_line->InitFromString(::GetCommandLineW()); // Use the full Unicode command line

    // APP
    CefRefPtr<NEONCEFsubApp> app(new NEONCEFsubApp());

    // Execute the subprocess logic.
    int exit_code = CefExecuteProcess(main_args, app.get(), nullptr);

    // LogToFile("I] NEONCEFsubApp END");

    // The subprocess has completed, return the exit code.
    return exit_code;
}
