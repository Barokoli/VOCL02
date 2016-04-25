#include "Eigen/Dense"

using namespace Eigen;

class Camera{
public:
	Vector3f pos;
    Vector3f dir;
    Vector3f right;
    Vector3f up;
    Vector3f scPos;
    Vector3f lookAt;

    int width,height;
    float speed;
    float tiltSpeed;

    float fov;
    float clipNear;
    float clipFar;

    bool drag;
    float dragStart[2];

    float kernelParams[18];
    cl_mem kernelParamsCLMem;
    Camera();
    Camera(int,int);
    Vector3f toFov(Vector3f,bool);
    void rotateSS (float deltaX,float deltaY);
    void updateParams();
};
Camera::Camera(){

}
Camera::Camera (int screenX,int screenY){
	width = screenX;
	height = screenY;

	speed = 1;
	tiltSpeed = 4;

    pos = Vector3f(5,5,80);

    lookAt = Vector3f(40,40,63);

    dir = lookAt-pos;

    dir.normalize();
    
    //dir = Vector3f(1,1,-30);
    right = dir.cross(Vector3f(0,0,1));
    up = right.cross(dir);

    std::cout << "Look direction vector: ("<< (float)dir[0] << ","<< (float)dir[1] << ","<< (float)dir[2] << ")" << std::endl;
    //cross cross
    fov = 60*0.0174533;

    dir.normalize();
    right.normalize();
    up.normalize();


    right = toFov(right,true);
    up = toFov(up,false);

    scPos = dir - right*0.5*width - up*0.5*height;

	clipNear = 0.01;
	clipFar = 1000;
	std::cout << "up direction vector: ("<< (float)up[0] << ","<< (float)up[1] << ","<< (float)up[2] << ")" << std::endl;
	std::cout << "right direction vector: ("<< (float)right[0] << ","<< (float)right[1] << ","<< (float)right[2] << ")" << std::endl;
	std::cout << "dir direction vector: ("<< (float)dir[0] << ","<< (float)dir[1] << ","<< (float)dir[2] << ")" << std::endl;
	std::cout << "scPos direction vector: ("<< (float)scPos[0] << ","<< (float)scPos[1] << ","<< (float)scPos[2] << ")" << std::endl;

	updateParams();
}
Vector3f Camera::toFov(Vector3f vctr,bool w){
  if(w){
    return vctr*((float)2.0*tan(fov*0.5)/width);
  }else{
	  //std::cout << (float)2.0*tan(((float)height/width)*fov*0.5)/height << std::endl;
	return vctr*((float)2.0*tan(((float)height/(float)width)*fov*0.5)/(float)height);
  }
}

void Camera::rotateSS(float deltaX,float deltaY){
	dir = dir*(1-(tiltSpeed*deltaX/(4.0f*width)+tiltSpeed*deltaY/(4.0f*height)))+(-right*deltaX)+up*deltaY;

	/*Quaternionf qr = Quaternionf(1,dir[0],dir[1],dir[2]);
	Quaternionf cam;
	cam.w() = 0;
	cam.vec() = dir;
	dir = ((Quaternionf)(qr*cam)).vec();*/
	dir.normalize();

	right = dir.cross(Vector3f(0,0,1));
	up = right.cross(dir);

	right.normalize();
	up.normalize();

	right = toFov(right,true);
	up = toFov(up,false);

	scPos = dir - right*0.5*width - up*0.5*height;

	updateParams();
}

void Camera::updateParams(){
	kernelParams[0] = pos[0];
	kernelParams[1] = pos[1];
	kernelParams[2] = pos[2];

	kernelParams[3] = dir[0];
	kernelParams[4] = dir[1];
	kernelParams[5] = dir[2];

	kernelParams[6] = up[0];
	kernelParams[7] = up[1];
	kernelParams[8] = up[2];

	kernelParams[9] = right[0];
	kernelParams[10] = right[1];
	kernelParams[11] = right[2];

	kernelParams[12] = scPos[0];
	kernelParams[13] = scPos[1];
	kernelParams[14] = scPos[2];

	kernelParams[15] = fov;
	kernelParams[16] = clipNear;
	kernelParams[17] = clipFar;
}
