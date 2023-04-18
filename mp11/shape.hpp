//partners: ykko2, mamir6


#ifndef SHAPE_H_
#define SHAPE_H_

#include<iostream>
#include<cmath>
#include<string>
#include<algorithm>
#include<list>
#include<fstream>
#include <iomanip>    

using namespace std;

//Base class
//Please implement Shape's member functions
//constructor, getName()
class Shape{
public:
	//Base class' constructor should be called in derived classes'
	//constructor to initizlize Shape's private variable 
  	Shape(string name) {
		name_ = name;
	}
	
  	string getName() {
		return name_;
	}
	
  	virtual double getArea() const = 0;
  	virtual double getVolume() const = 0;
	
private:
  string name_;


};

//Rectangle
//Please implement the member functions of Rectangle:
//constructor, getArea(), getVolume(), operator+, operator-

template <class T>
class Rectangle : public Shape{
public:
  	Rectangle<T>(T width = 0, T length = 0):Shape("Rectangle") {
		width_ = width;
		length_ = length;
	}
	
  	double getArea() const {
		return width_ * length_;
	}
	
  	double getVolume() const {
		return 0;
	}
	
	Rectangle<T> operator + (const Rectangle<T>& rec) {
		Rectangle New = Rectangle(width_ + rec.getWidth(), length_ + rec.getLength());
		return New;
	}
	
	Rectangle<T> operator - (const Rectangle<T>& rec) {
		double newLength = getLength() - rec.length_;
		double newWidth = getWidth() - rec.width_;
		if (newWidth <0){
			newWidth = 0;
		}
		if (newLength <0){
			newLength = 0;
		}
		Rectangle NewRect = Rectangle(newWidth,newLength);
		return NewRect;
	} 
	
	T getWidth() const { 
		return width_;
	}
	
	T getLength() const { 
		return length_;
	}
private:
	T width_;
        T length_;

};

//Circle
//Please implement the member functions of Circle:
//constructor, getArea(), getVolume(), operator+, operator-
//@@Insert your code here

class Circle : public Shape{
public:
  	Circle(double radius):Shape("Circle") {
		radius_ = radius;
	}
	
  	double getArea() const{
		return M_PI * pow(radius_,2);
	}
	
 	double getVolume() const{
		return 0;
	}
	
  	Circle operator + (const Circle& cir){
		Circle NewCircle = Circle(radius_ + cir.getRadius());
		return NewCircle;
	}
	
	Circle operator - (const Circle& cir){
		double newRadius = getRadius() - cir.getRadius();
		if (newRadius <0){
			newRadius = 0;
		}
		Circle NewCircle = Circle(newRadius);
		return NewCircle;
	} 

	double getRadius() const { 
		return radius_;
	}
	
private:
	double radius_;

};



//Sphere
//Please implement the member functions of Sphere:
//constructor, getArea(), getVolume(), operator+, operator-
//@@Insert your code here

class Sphere : public Shape{
public:
  	Sphere(double radius):Shape("Sphere") {
		radius_ = radius;
	}

  	double getVolume() const {
		return (4.0/3.0) * M_PI * pow(radius_,3);
	}	
	
  	double getArea() const {
		return 4.0* M_PI * pow(radius_,2); 
	}

	Sphere operator + (const Sphere& sph) {
		Sphere NewSphere = Sphere(radius_ + sph.getRadius());
		return NewSphere;
	}

	Sphere operator - (const Sphere& sph) {
		double newRadius = getRadius() - sph.getRadius();
		if (newRadius <0){
			newRadius = 0;
		}
		Sphere NewSphere = Sphere(newRadius);
		return NewSphere;
	} 
	
	double getRadius() const {
		return radius_;
	}

private:
	double radius_;

};

//Rectprism
//Please implement the member functions of RectPrism:
//constructor, getArea(), getVolume(), operator+, operator-
//@@Insert your code here
class RectPrism : public Shape{
public:
  	RectPrism(double width, double length, double height):Shape("RectPrism") {
		width_ = width; 
		length_ = length; 
		height_ = height;
	}
	
  	double getVolume() const {
		return width_ * length_ * height_;
	}
  	
	double getArea() const {
		return 2* (length_ * height_ + width_ * height_ + length_ * width_);
	}
	
	RectPrism operator + (const RectPrism& rectp){
		RectPrism NewRectPrism = RectPrism(width_ + rectp.getWidth(), length_ + rectp.getLength(), height_ + rectp.getHeight());
		return NewRectPrism;
	}
	
	RectPrism operator - (const RectPrism& rectp){
		double newLength = getLength() - rectp.length_;
		double newWidth = getWidth() - rectp.width_;
		double newHeight = getHeight() - rectp.height_;
		if (newWidth <0){
			newWidth = 0;
		}
		if (newLength <0){
			newLength = 0;
		}
		if (newHeight <0){
			newHeight = 0;
		}
		RectPrism NewRectPrism = RectPrism(newWidth,newLength,newHeight);
		return NewRectPrism;
	}	
	
	double getWidth() const { 
		return width_;
	}
	
	double getLength() const { 
		return length_;
	}
	
	double getHeight() const { 
		return height_;
	}
  
private:
  	double length_;
  	double width_;
  	double height_;

};

// Read shapes from test.txt and initialize the objects
// Return a vector of pointers that points to the objects 
static list<Shape*> CreateShapes(char* file_name) {
	//@@Insert your code here
	string name;
	int count;
	list<Shape*> shapeslist;
	ifstream file (file_name,std::ifstream::in);
	file >> count;
	for(int i = 0; i<count; i++){
		file >> name;
		if (name=="Rectangle"){
			double l,w;
			file>>l>>w;
			Shape* RecRead = new Rectangle<double>(l,w);
			shapeslist.push_back(RecRead);
		}
		else if (name=="Circle"){
			double rad;
			file>>rad;
			Shape* CirRead = new Circle(rad);
			shapeslist.push_back(CirRead);
		}
		else if (name=="Sphere"){
			double rad;
			file>>rad;
			Shape *SphRead = new Sphere(rad);
			shapeslist.push_back(SphRead);
		}
		else if (name=="RectPrism"){
			double l,w,h;
			file>>l>>w>>h;
			Shape *RpRead = new RectPrism(l,w,h);
			shapeslist.push_back(RpRead);
		}
	}
	file.close();
	return shapeslist;
}

// call getArea() of each object 
// return the max area
static double MaxArea(list<Shape*> shapes){
	double max_area = 0;
	//@@Insert your code here
	list<Shape*>::iterator it;
    for(list<Shape*>::iterator it = shapes.begin(); it != shapes.end(); it++){
        if((*it)->getArea()>max_area){
			max_area = (*it)->getArea();
		}
    }
    return max_area;
}

// call getVolume() of each object 
// return the max volume
static double MaxVolume(list<Shape*> shapes){
	double max_volume = 0;
	//@@Insert your code here
	list<Shape*>::iterator it = shapes.begin();
    for(list<Shape*>::iterator it =shapes.begin(); it != shapes.end(); it++){
        if((*it)->getVolume()>max_volume){
			max_volume = (*it)->getVolume();
		}
    }
	return max_volume;
}
#endif