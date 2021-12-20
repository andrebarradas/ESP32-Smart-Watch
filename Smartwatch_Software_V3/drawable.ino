#include "drawable.h"

Drawable::Drawable(int x, int y, int width, int height, GFXcanvas16 *buffer_ptr)
{
    _x = x;
    _y = y;
    _width = width;
    _height = height;
    _buffer_ptr = buffer_ptr;
}
 Drawable::Drawable(int x, int y, int width, int height, GFXcanvas16 *buffer_ptr, String type){
    _x = x;
    _y = y;
    _width = width;
    _height = height;
    _buffer_ptr = buffer_ptr;
    _type = type;  
 }

void Drawable::draw()
{
    _buffer_ptr->fillRect(_x, _y, _width, _height, 0xFFFF);
    _buffer_ptr->drawRect(_x, _y, _width, _height, 0x0000);
}
int Drawable::getx() {
    return _x; }

int Drawable::gety() {
    return _y; }

int Drawable::getWidth() {
    return _width; }

int Drawable::getHeight() {
    return _height; }
    
boolean Drawable::isTouched(int x, int y)
{
    boolean touched = (x >= _x) && (y >= _y) && (x <= _x + _width) && (y <= _y + _height) && _touchable;
    if (touched)
        onTouch();
    return touched; 
}
void Drawable::onTouch()
{
    printDebug("Touch detected on " + toString());
    if (_callback != nullptr)
        _callback();
}
void Drawable::setTouchable(boolean val){
    _touchable = val;}

void Drawable::registerCallback(void (*cb)()){
    _callback = cb;
}

String Drawable::toString(){
    return _type + "@x:" + String(_x) + "y:" + String(_y) + " with width=" + String(_width) + " height=" + String(_height); 
}