#include "iostream"
#include "dlfcn.h" 
#include "KinovaTypes.h"
#include "Kinova.API.CommLayerUbuntu.h"
#include "math.h"
#include "vector"
int main()
{
        int result;
        std::cout << "SendBasicTrajectory function example" << std::endl;
        //Handle for the library's command layer.
        void * commandLayer_handle;
        //Function pointers to the functions we need
        int (*MyInitAPI)();
        int (*MyCloseAPI)();
        int (*MySendBasicTrajectory)(TrajectoryPoint command);
        int (*MyStartControlAPI)();
        //We load the library (Under Windows, use the function LoadLibrary)
        commandLayer_handle = dlopen("Kinova.API.USBCommandLayerUbuntu.so",RTLD_NOW|RTLD_GLOBAL);
        //We load the functions from the library (Under Windows, use GetProcAddress)
        MyInitAPI = (int (*)()) dlsym(commandLayer_handle,"InitAPI");
        MyCloseAPI = (int (*)()) dlsym(commandLayer_handle,"CloseAPI");
        MySendBasicTrajectory = (int (*)(TrajectoryPoint)) dlsym(commandLayer_handle,"SendBasicTrajectory");
        MyStartControlAPI = (int (*)()) dlsym(commandLayer_handle,"StartControlAPI");
        //If the was loaded correctly
        if((MyInitAPI == NULL) || (MyCloseAPI == NULL) || (MySendBasicTrajectory == NULL) || (MyStartControlAPI == NULL))
        {
                std::cout << "Unable to initialize the command layer." << std::endl;
        }
        else
        {
                std::cout << "The command has been initialized correctly." << std::endl << std::endl;
                std::cout << "Calling the method InitAPI()" << std::endl;
                result = (*MyInitAPI)();
                std::cout << "result of InitAPI() = " << result << std::endl << std::endl;
                //We prepare the virtual joystick command that will be sent to the robotic arm.
                TrajectoryPoint trajectoryPoint;
                trajectoryPoint.InitStruct();

                trajectoryPoint.Position.HandMode = HAND_NOMOVEMENT;
                trajectoryPoint.Position.Type = CARTESIAN_POSITION;

                std::vector<double> pose_value(6);
                pose_value[0] = 0.18867;
                pose_value[3] = 1.63722;
                pose_value[4] = 0.947005;
                pose_value[5] = 0.144803;
         
                double y_center = -0.331953;
                double z_center = 0.505702;              
                double angle= 0;
                double radius = 0.1;            
                double angle_resolution = 10;                /* 两个采样点间的角度值，减小可提高轨迹规划成功率 */
                double d_angle = angle_resolution*3.14/180; /* 两个采样点间的弧度值 */
             
                //  采样圆上的点
                for (int i= 0; i< (360/angle_resolution); i++)
                {
                    angle+= d_angle; 
                    pose_value[1] = y_center + radius*sin(angle);
                    pose_value[2] = z_center + radius*cos(angle); 
                    trajectoryPoint.Position.CartesianPosition.X = pose_value[0];
                    trajectoryPoint.Position.CartesianPosition.Y = pose_value[1];
                    trajectoryPoint.Position.CartesianPosition.Z = pose_value[2];
                    //We set the orientation part of the position (unit is RAD)
                    trajectoryPoint.Position.CartesianPosition.ThetaX = pose_value[3];
                    trajectoryPoint.Position.CartesianPosition.ThetaY = pose_value[4];
                    trajectoryPoint.Position.CartesianPosition.ThetaZ = pose_value[5];
                    (*MySendBasicTrajectory)(trajectoryPoint);       
                    // printf("%f %f %f %f %f %f %f \n",pose_value[0],pose_value[1],pose_value[2],pose_value[3],pose_value[4],pose_value[5],pose_value[6]);
                }   

                        // std::cout << std::endl << "Calling the method CloseAPI()" << std::endl;
                        // result = (*MyCloseAPI)();
                        // std::cout << "result of CloseAPI() = " << result << std::endl;
                }
                return 0;
}
//g++ test2.cpp -o test2.out -ldl
