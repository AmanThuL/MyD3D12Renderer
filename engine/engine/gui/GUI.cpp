//*******************************************************************
// GUI.cpp
//*******************************************************************

#include "GUI.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void GUI::Init()
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    //Load Fonts
    //- If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    //- AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    //- If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    //- The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    //- Read 'docs/FONTS.md' for more instructions and details.
    //- Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontFromFileTTF("engine/resources/fonts/FiraCode-Retina.ttf", 16.0f);

    //io.ConfigWindowsResizeFromEdges = true;
}

void GUI::SetupPlatform(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam);

    //mGuiIO& io = ImGui::GetIO();
    //if (io.WantCaptureMouse && (msg == WM_LBUTTONDOWN || msg == WM_LBUTTONUP || 	
    //							msg == WM_RBUTTONDOWN || msg == WM_RBUTTONUP || 
    //							msg == WM_MBUTTONDOWN || msg == WM_MBUTTONUP || 
    //							msg == WM_MOUSEWHEEL  || msg == WM_MOUSEMOVE))
    //{
    //	return true;
    //}
}

void GUI::SetupRenderer(ID3D12Device* device, DescriptorHeapWrapper* descHeap)
{
    ImGui_ImplDX12_Init(device, gNumFrameResources,
        DXGI_FORMAT_R8G8B8A8_UNORM, descHeap->GetHeapPtr(),
        descHeap->GetCPUHandle(descHeap->GetLastDescIndex()),
        descHeap->GetGPUHandle(descHeap->GetLastDescIndex()));
}

void GUI::StartFrame()
{
    // Start the Dear ImGui frame
    ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    // Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
    {
        static int imCounter = 0;

        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            imCounter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", imCounter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }
}

void GUI::RenderFrame(ID3D12GraphicsCommandList* commandList, DescriptorHeapWrapper* descHeap)
{
    // Rendering
    ImGui::Render();

    // Set the constant buffer tables to the command list.
    ID3D12DescriptorHeap* descriptorHeaps[] = { descHeap->GetHeapPtr() };
    commandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList);
}

void GUI::ShutDown()
{
    // Clean up
    ImGui_ImplDX12_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}
