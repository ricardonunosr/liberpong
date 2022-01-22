/*
	_        __           _  __     __                                          __  __
   (_)  ____/ /___  ____ ( )/ /_   / /______  ____ _      __   ____ ___  ____ _/ /_/ /_
  / /  / __  / __ \/ __ \|// __/  / //_/ __ \/ __ \ | /| / /  / __ `__ \/ __ `/ __/ __ \
 / /  / /_/ / /_/ / / / / / /_   / ,< / / / / /_/ / |/ |/ /  / / / / / / /_/ / /_/ / / /
/_/   \__,_/\____/_/ /_/  \__/  /_/|_/_/ /_/\____/|__/|__/  /_/ /_/ /_/\__,_/\__/_/ /_/

*/
#pragma once

union vec4 {
    float elements[4];
    
    // First Element
    union{
        float x,r;
    };
    
    // Second Element
    union{
        float y,g;
    };
    
    // Third Element
    union{
        float z,b;
    };
    
    // Fourth Element
    union{
        float w,a;
    };
    
	vec4() :x(0), y(0), z(0), w(0) {}
    
    vec4(float value):x(value),y(value),z(value),w(value){}
    
	vec4(float x, float y, float z, float w) :x(x), y(y), z(z), w(w) {}
    
    
    // NOTE(Ricardo): Maybe we can have negative rows but for now its only positive rows. 
    float& operator[](unsigned int row)
    {
        float* address = (float*)this;
        return address[row];
    };
};

union mat4 {
	float array[16];
    
	vec4 columns[4];
    
	mat4()
	{
		columns[0] = vec4(0.0f);
		columns[1] = vec4(0.0f);
		columns[2] = vec4(0.0f);
		columns[3] = vec4(0.0f);
	}
    
    mat4(float value){
        columns[0] = vec4(value);
		columns[1] = vec4(value);
		columns[2] = vec4(value);
		columns[3] = vec4(value);
    }
    
};


inline mat4 ProjectionMatrixPerspective(float fovy, float aspectRatio, float nearPlan, float farPlane);

inline mat4 ProjectionMatrixOrtographic(float left,float right,float bottom, float top);
/*

  _____                 _                           _        _   _             
 |_   _|               | |                         | |      | | (_)            
   | |  _ __ ___  _ __ | | ___ _ __ ___   ___ _ __ | |_ __ _| |_ _  ___  _ __  
   | | | '_ ` _ \| '_ \| |/ _ \ '_ ` _ \ / _ \ '_ \| __/ _` | __| |/ _ \| '_ \ 
  _| |_| | | | | | |_) | |  __/ | | | | |  __/ | | | || (_| | |_| | (_) | | | |
 |_____|_| |_| |_| .__/|_|\___|_| |_| |_|\___|_| |_|\__\__,_|\__|_|\___/|_| |_|
                 | |                                                           
                 |_|                                                           

*/



inline mat4 ProjectionMatrixPerspective(float fovy, float aspectRatio, float nearPlan, float farPlane){
    mat4 matrix = mat4();
    
    return matrix;
    
}

inline mat4 ProjectionMatrixOrtographic(float left,float right,float bottom, float top)
{
    
}
