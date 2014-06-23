#include "util.h"
#include <vector>

using namespace cocos2d;

int color[][3] = {
    {255, 0, 0},
    {255, 100, 0},
    {255, 0, 100},
    {255, 100, 100},

    {0, 255, 0},
    {100, 255, 0},
    {0, 255, 100},
    {100, 255, 100},

    {0, 0, 255},
    {100, 0, 255},
    {0, 100, 255},
    {100, 100, 255},
};

void putpixel(unsigned char *data, int width , int height, int x, int y, int col) {
    x = std::max(std::min(width-1, x), 0);
    y = std::max(std::min(height-1, y), 0);
    int id = y*width+x;
    int r, g, b;
    r = g = b = 0;
    int idx = col%12;
    r = color[idx][0];
    g = color[idx][1];
    b = color[idx][2];

    /*
    if(col % 12 == 0) {
        r = 255;
    } else if(col % 3 == 1) {
        g = 255;
    } else {
        b = 255;
    }
    */

    data[id*4+0] = r;
    data[id*4+1] = g;
    data[id*4+2] = b;
    data[id*4+3] = 255;
}

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

//扫描线fill 算法
void drawTriangle(unsigned char *data, float *depth, int width, int height, int x1, int y1, float z1, int x2, int y2, float z2, int x3, int y3, float z3, int col){
    CCLog("drawTriangle %d %d %f %d %d %f %d %d %f", x1, y1, z1, x2, y2, z2, x3, y3, z3);
    int temp;
    if(y1 > y2) {
        swap(&x1, &x2);
        swap(&y1, &y2);
    }
    if(y2 > y3) {
        swap(&x2, &x3);
        swap(&y2, &y3);
    }
    if(y1 > y2) {
        swap(&x1, &x2);
        swap(&y1, &y2);
    }

    CCLog("drawTriangle2 %d %d %d %d %d %d", x1, y1, x2, y2, x3, y3 );
    //CCLog("%f %f", x2-x1, y2-y1); 
    
    float dx_far = (x3-x1)*1.0f/(y3-y1+1);
    float dx_upper = (x2-x1)*1.0f/(y2-y1+1);
    float dx_low = (x3-x2)*1.0f/(y3-y2+1);
    

    float dz_far = (z3-z1)*1.0f/(y3-y1+1);
    float dz_upper = (z2-z1)*1.0f/(y2-y1+1);
    float dz_low = (z3-z2)*1.0f/(y3-y2+1);

    
    float xf = x1;
    float xt = x1+dx_upper;

    float zf = z1;
    float zt = z1+dz_upper;

    
    float maxY = y3 > height-1? height-1: y3;


    CCLog("dx dy %f %f %f", dx_far, dx_upper, dx_low);
    CCLog("xf xt %f %f", xf, xt);
    //线性插值得到 z 值
    int vx1 = x3-x1;
    int vy1 = y3-y1;
    int vx2 = x2-x1;
    int vy2 = y2-y1;

    //float delta = vx1*vy2-vx2*vy1;

    for(int y=y1; y <= maxY; y++) {

        float minX = xf > 0 ? xf: 0;
        float maxX = xt < width-1? xt: width-1;
        float sz = zf;
        float ez = zt;
        for(int x=minX; x <= maxX; x++) {
            /*
            float ax, by;
            float dx = x-x1;
            float dy = y-y1;

            float sx = dx*vy2-vx2*dy;
            float sy = vx1*dy-dx*vy1;

            ax = sx/delta;
            by = sy/delta;
            float newDepth = ax*(z3-z1)+by*(z2-z1)+z1;
            */

            float newDepth = sz + (ez-sz)*(x-minX)/(maxX-minX+1);
            float oldDepth = depth[y*width+x];

            //靠近 屏幕更近的点
            if(oldDepth == 0 ||newDepth < oldDepth) {
                depth[y*width+x] = newDepth;
                putpixel(data, width, height, x, y, col);
            }
        }
        
        maxX = xf < width-1? xf: width-1;
        minX = xt > 0? xt: 0;

        sz = zt;
        ez = zf;

        for(int x=maxX; x >= minX; x--) {
            /*
            float ax, by;
            float dx = x-x1;
            float dy = y-y1;

            float sx = dx*vy2-vx2*dy;
            float sy = vx1*dy-dx*vy1;

            ax = sx/delta;
            by = sy/delta;
            float newDepth = ax*(z3-z1)+by*(z2-z1)+z1;
            */
            float newDepth = sz + (ez-sz)*(x-minX)/(maxX-minX+1);

            float oldDepth = depth[y*width+x];
            if(oldDepth == 0 || newDepth < oldDepth) {
                depth[y*width+x] = newDepth;
                putpixel(data, width, height, x, y, col);
            }

            //putpixel(data, width, height, x, y, col);
        }

        xf += dx_far;
        zf += dz_far;
        if(y < y2) {
            xt += dx_upper;
            zt += dz_upper;
        }else {
            xt += dx_low;
            zt += dz_low;
        }
    }
}



//c color 
void drawLine(unsigned char *data, int width, int height, int x1, int y1, int x2, int y2, int col) {
  int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
  CCLog("draw Line %d %d %d %d", x1, y1, x2, y2);
  dx = x2 - x1;
  dy = y2 - y1;
  dx1 = fabs(dx);
  dy1 = fabs(dy);
  px = 2 * dy1 - dx1;
  py = 2 * dx1 - dy1;
  if (dy1 <= dx1) {
    if (dx >= 0) {
      x = x1;
      y = y1;
      xe = x2;
    } else {
      x = x2;
      y = y2;
      xe = x1;
    }
    putpixel(data, width, height, x, y, col);
    
    for (i = 0; x < xe; i++) {
      x = x + 1;
      if (px < 0) {
        px = px + 2 * dy1;
      } else {
        if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) {
          y = y + 1;
        } else {
          y = y - 1;
        }
        px = px + 2 * (dy1 - dx1);
      }
      //delay(0);
      putpixel(data, width, height, x, y, col);
    }
  } else {
    if (dy >= 0) {
      x = x1;
      y = y1;
      ye = y2;
    } else {
      x = x2;
      y = y2;
      ye = y1;
    }
    putpixel(data, width, height, x, y, col);
    for (i = 0; y < ye; i++) {
      y = y + 1;
      if (py <= 0) {
        py = py + 2 * dx1;
      } else {
        if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) {
          x = x + 1;
        } else {
          x = x - 1;
        }
        py = py + 2 * (dx1 - dy1);
      }
      putpixel(data, width, height, x, y, col);
    }
  }
}
