#include <iostream>
#include <dlfcn.h> //Ubuntu
#include <KinovaTypes.h>
#include <Kinova.API.CommLayerUbuntu.h>
//Note that under windows, you may/will have to perform other #include
using namespace std;
int main()
{
        int result;
        CartesianPosition data;
        cout << "GetCartesianPosition function example" << endl;
        //Handle for the library's command layer.
        void * commandLayer_handle;
        //Function pointers to the functions we need
        int (*MyInitAPI)();
        int (*MyCloseAPI)();
        int (*MyGetCartesianPosition)(CartesianPosition &);
        //We load the library (Under Windows, use the function LoadLibrary)
        commandLayer_handle = dlopen("Kinova.API.USBCommandLayerUbuntu.so",RTLD_NOW|RTLD_GLOBAL);
        //We load the functions from the library (Under Windows, use GetProcAddress)
        MyInitAPI = (int (*)()) dlsym(commandLayer_handle,"InitAPI");
        MyCloseAPI = (int (*)()) dlsym(commandLayer_handle,"CloseAPI");
        MyGetCartesianPosition = (int (*)(CartesianPosition &)) dlsym(commandLayer_handle,"GetCartesianPosition");
        //If the was loaded correctly
        if((MyInitAPI == NULL) || (MyCloseAPI == NULL) || (MyGetCartesianPosition == NULL))
        {
                cout << "Unable to initialize the command layer." << endl;
        }
        else
        {
                cout << "The command has been initialized correctly." << endl << endl;
                cout << "Calling the method InitAPI()" << endl;
                result = (*MyInitAPI)();
                cout << "result of InitAPI() = " << result << endl;
                result = (*MyGetCartesianPosition)(data);
                cout << "       Position X : " << data.Coordinates.X << endl;
                cout << "       Position Y : " << data.Coordinates.Y << endl;
                cout << "       Position Z : " << data.Coordinates.Z << endl;
                cout << "  Position ThetaX : " << data.Coordinates.ThetaX << endl;
                cout << "  Position ThetaY : " << data.Coordinates.ThetaY << endl;
                cout << "  Position ThetaZ : " << data.Coordinates.ThetaZ << endl;
                cout << "Position Finger 1 : " << data.Fingers.Finger1 << endl;
                cout << "Position Finger 2 : " << data.Fingers.Finger2 << endl;
                cout << "Position Finger 3 : " << data.Fingers.Finger3 << endl;
                cout << endl << "Calling the method CloseAPI()" << endl;
                result = (*MyCloseAPI)();
                cout << "result of CloseAPI() = " << result << endl;
        }
        return 0;
}