#include <functional>
#include <vector>
#include <mutex>

#define Pegasos_GLFW

#ifdef Pegasos_GLFW
struct GLFWwindow;
typedef GLFWwindow WindowType;
#endif

typedef std::function<void()> VoidCallback;
typedef std::vector<VoidCallback*> CallbacksVector;

namespace Pegasos{
    struct WindowDetails{
        uint32_t height, width;
        char* windowName;
    };

    class Window
    {
    protected:
        WindowType* handler;
        WindowDetails details;

        bool innerClose;
        std::mutex callbackMutex;

        CallbacksVector resizeCallbacks;
        CallbacksVector closeCallbacks;
        CallbacksVector keyboardCallbacks;
        CallbacksVector cycleCallbacks;


    public:
        WindowType* getHandler();
        void run();    

        bool addCycleCallback(VoidCallback* callback);
        bool rmCycleCallback(VoidCallback* callback);            

        // void addResizeCallback(VoidCallback* callback);
        // void rmResizeCallback(VoidCallback* callback);

        // void addCloseCallback(VoidCallback* callback);
        // void rmCloseCallback(VoidCallback* callback);

        // void addKeyboardCallback(VoidCallback* callback);
        // void rmKeyboardCallback(VoidCallback* callback);
        
        Window(char* windowName, uint32_t height, uint32_t width, uint32_t apiHint = 0);
        ~Window();
    };

};