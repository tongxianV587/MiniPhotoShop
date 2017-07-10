//
// Created by Raye on 2017/7/6.
//

#include "Model.h"
#include <QDebug>
#include <iostream>
#include <fstream>
Model::Model(){
    NowDoneIndex=-1;
    MaxDoneIndex=-1;
}

void Model::addLine(double centerX,double centerY,double x1,double y1,double x2,double y2)
{
    shared_ptr<Line> pLine;
    layouts.list.push_back(pLine=shared_ptr<Line>(new Line(centerX,
        centerY,SHAPE::LINE,string("Line"),1.0,1.0,0.0,pen,x1,y1,x2,y2)));
    qDebug()<<centerX<<centerY<<x1<<y1<<x2<<y2;
    addDoneEvent(COMMAND::CREATE,layouts.list.size()-1,shared_ptr<BaseShape>(new Line(*pLine)));
    Params params;
    params.setType(NOTIFY::UPDATE_IMAGE_ADD);
    params.setInts({(int)layouts.list.size()-1});
    notify(params);
}
void Model::addEllipse(double centerX,double centerY,double a,double b){
    shared_ptr<Ellipse> pEllipse;
    layouts.list.push_back(pEllipse=shared_ptr<Ellipse>(new Ellipse(centerX,
        centerY,SHAPE::ELLIPSE,string("Ellipse"),1.0,1.0,0.0,pen,brush,a,b)));
    addDoneEvent(COMMAND::CREATE,layouts.list.size()-1,shared_ptr<BaseShape>(new Ellipse(*pEllipse)));

    Params params;
    params.setType(NOTIFY::UPDATE_IMAGE_ADD);
     params.setInts({(int)layouts.list.size()-1});
    notify(params);
}

void Model::addRect(double centerX, double centerY, double width, double height)
{
    shared_ptr<Rect> pRect;
    layouts.list.push_back(pRect = shared_ptr<Rect>(new Rect(centerX,
         centerY, SHAPE::RECT, string("Rectangle"),1.0,1.0,0,pen,brush,width,height)));
    addDoneEvent(COMMAND::CREATE,layouts.list.size()-1,shared_ptr<BaseShape>(new Rect(*pRect)));

    Params params;
    params.setType(NOTIFY::UPDATE_IMAGE_ADD);
    params.setInts({(int)layouts.list.size()-1});
    notify(params);
}
void Model::addBaseShape(vector<shared_ptr<BaseShape>>::iterator  it,shared_ptr<BaseShape> shape){
    //Redo Need it.
    if(shape!=nullptr){
        layouts.list.insert(it,NewBaseShape(shape));
    }
}
shared_ptr<BaseShape> Model::NewBaseShape(shared_ptr<BaseShape> shape){
    shared_ptr<BaseShape> newBaseShape=nullptr;
    if(shape!=nullptr){
        string name=shape->getName();
        if(name=="Line"){
            newBaseShape=shared_ptr<BaseShape>(new Line(*(static_pointer_cast<Line>(shape))));
        }else if(name=="Ellipse"){
            newBaseShape=shared_ptr<BaseShape>(new Ellipse(*(static_pointer_cast<Ellipse>(shape))));
        }else if(name=="Rectangle"){
            newBaseShape=shared_ptr<BaseShape>(new Rect(*(static_pointer_cast<Rect>(shape))));
        }else if(name=="image"){
            newBaseShape=shared_ptr<BaseShape>(new Pixmap(*(static_pointer_cast<Pixmap>(shape))));
        }else{
            qDebug()<<"Cannot recognize this shape name :"<<QString(name.c_str());
        }
    }else{
        qDebug()<<"The shape is null";
    }
    return newBaseShape;
}
 void Model::SetPenColor(unsigned char r,unsigned char g,unsigned char b)
 {
     pen.setForeR(r);
     pen.setForeG(g);
     pen.setForeB(b);
 }

 void Model::SetBrushColor(unsigned char r,unsigned char g,unsigned char b)
 {
     brush.setBackR(r);
     brush.setBackG(g);
     brush.setBackB(b);
 }

 void Model::ClearModel()
 {
     if(!layouts.list.empty())
     {
         layouts.list.clear();
     }
 }

 bool Model::isProjectEmpty()const
 {
     return layouts.list.empty();
 }

 bool Model::newProject(bool isSavedPre)
 {
     if(!isSavedPre)
         return false;

     if(!isProjectEmpty())
         ClearModel();
     return true;
 }

 bool Model::saveProject(string path)const
 {
     ofstream out;
     Pen pen;
     Brush brush;
     shared_ptr<Line> line;
     shared_ptr<Ellipse> ellipse;
     shared_ptr<Rect> rect;
     int i;
     int num = layouts.list.capacity() - 1;

     out.open(path);
     if(!out)
     {
         return false;
     }

     out << "This is a mpsd project" << endl;
     for(i = 0; i < num; i++)
     {
         out << layouts.list[i]->getType();
         switch(layouts.list[i]->getType())
         {
         case SHAPE::LINE:
             line = shared_ptr<Line>(static_pointer_cast<Line>(layouts.list[i]));
             out << line->getPosX() << line->getPosY() << line->getName() << line->getScaleX() << line->getScaleY() << line->getAngle();
             pen = line->getPen();
             out << pen.getForeR() << pen.getForeG() << pen.getForeB() << pen.getLineWidth() << pen.getPenStyle();
             out << line->getX1() << line->getY1() << line->getX2() << line->getY2() << endl;
             break;

         case SHAPE::ELLIPSE:
             ellipse = shared_ptr<Ellipse>(static_pointer_cast<Ellipse>(layouts.list[i]));
             out << ellipse->getPosX() << ellipse->getPosY() << ellipse->getName() << ellipse->getScaleX() << ellipse->getScaleY() << ellipse->getAngle();
             pen = ellipse->getPen();
             out << pen.getForeR() << pen.getForeG() << pen.getForeB() << pen.getLineWidth() << pen.getPenStyle();
             brush = ellipse->getBrush();
             out << brush.getBackR() << brush.getBackG() << brush.getBackB() << brush.getBrushStyle();
             out << ellipse->getA() << ellipse->getB() << endl;
             break;

         case SHAPE::RECT:
             rect = shared_ptr<Rect>(static_pointer_cast<Rect>(layouts.list[i]));
             out << rect->getPosX() << rect->getPosY() << rect->getName() << rect->getScaleX() << rect->getScaleY() << rect->getAngle();
             pen = rect->getPen();
             out << pen.getForeR() << pen.getForeG() << pen.getForeB() << pen.getLineWidth() << pen.getPenStyle();
             brush = rect->getBrush();
             out << brush.getBackR() << brush.getBackG() << brush.getBackB() << brush.getBrushStyle();
             out << rect->getWidth() << rect->getHeight() << endl;
             break;

         case SHAPE::PIXMAP:
             break;
         }
     }

     out.close();
     return true;
 }

 bool Model::loadProject(string path)
 {
     ifstream in;
     int type;
     char head[30];
     char t;
     int PosX, PosY, x1, y1, x2, y2, a, b, width, height, penStyle, lineWidth, brushstyle;
     unsigned char R, G, B;
     double scaleX, scaleY, angle;
     string name;
     Params params;
     Pen pen;
     Brush brush;
     shared_ptr<Line> pLine;
     shared_ptr<Ellipse> pEllipse;
     shared_ptr<Rect> pRect;

     if(!isProjectEmpty())
         ClearModel();

     in.open(path);
     if(!in)
         return false;

     in.getline(head, 30, '\n');
     if(strcmp(head, "This is a mpsd project") != 0)
         return false;

     while(!in.eof())
     {
         in >> type;
         switch(type)
         {
         case SHAPE::LINE:
             in >> PosX >> PosY >> name >> scaleX >> scaleY >> angle;
             in >> R >> G >> B >> lineWidth >> penStyle;
             pen.setForeR(R);
             pen.setForeG(G);
             pen.setForeB(B);
             pen.setLineWidth(lineWidth);
             pen.setPenStyle(penStyle);
             in >> x1 >> y1 >> x2 >> y2 >> t;
             layouts.list.push_back(pLine=shared_ptr<Line>(new Line(PosX,
                 PosY,SHAPE::LINE,name,scaleX,scaleY,angle,pen,x1,y1,x2,y2)));
             params.setType(NOTIFY::UPDATE_IMAGE_ADD);
             params.setInts({(int)layouts.list.size()-1});
             notify(params);
             break;

         case SHAPE::ELLIPSE:
             in >> PosX >> PosY >> name >> scaleX >> scaleY >> angle;
             in >> R >> G >> B >> lineWidth >> penStyle;
             pen.setForeR(R);
             pen.setForeG(G);
             pen.setForeB(B);
             pen.setLineWidth(lineWidth);
             pen.setPenStyle(penStyle);
             in >> R >> G >> B >> brushstyle;
             brush.setBackR(R);
             brush.setBackG(G);
             brush.setBackB(B);
             brush.setBrushStyle(brushstyle);
             in >> a >> b >> t;
             layouts.list.push_back(pEllipse=shared_ptr<Ellipse>(new Ellipse(PosX,
                 PosY,SHAPE::ELLIPSE,name,scaleX,scaleY,angle,pen,brush,a,b)));
             params.setType(NOTIFY::UPDATE_IMAGE_ADD);
              params.setInts({(int)layouts.list.size()-1});
             notify(params);
             break;

         case SHAPE::RECT:
             in >> PosX >> PosY >> name >> scaleX >> scaleY >> angle;
             in >> R >> G >> B >> lineWidth >> penStyle;
             pen.setForeR(R);
             pen.setForeG(G);
             pen.setForeB(B);
             pen.setLineWidth(lineWidth);
             pen.setPenStyle(penStyle);
             in >> R >> G >> B >> brushstyle;
             brush.setBackR(R);
             brush.setBackG(G);
             brush.setBackB(B);
             brush.setBrushStyle(brushstyle);
             in >> width >> height >> t;
             layouts.list.push_back(pRect = shared_ptr<Rect>(new Rect(PosX,
                  PosY, SHAPE::RECT, name,scaleX,scaleY,angle,pen,brush,width,height)));
             params.setType(NOTIFY::UPDATE_IMAGE_ADD);
             params.setInts({(int)layouts.list.size()-1});
             notify(params);
             break;

         case SHAPE::PIXMAP:
             break;
         }
     }

     in.close();
     return true;
 }

 void Model::addImage(string fileName)
 {
     shared_ptr<Pixmap> newImage(new Pixmap(string("image"),fileName));
    if(newImage->GetFormat()==PIXMAP::FMT_NULL)
    {
        Params params;
        params.setType(NOTIFY::ADD_IMAGE_FAILED);
        notify(params);
        return;
    }
    layouts.list.push_back(newImage);
    addDoneEvent(COMMAND::CREATE,layouts.list.size()-1,shared_ptr<BaseShape>(new Pixmap(*newImage)));
    Params params;
    params.setType(NOTIFY::UPDATE_IMAGE_ADD);
    params.setInts({(int)layouts.list.size()-1});
    notify(params);
 }

 void Model::addDoneEvent(int commandtype,int layoutindex,shared_ptr<BaseShape> aftershape,shared_ptr<BaseShape> beforeshape){
    //delete ->before valid create ->after valid modify before after valid
     //add an event
    if(NowDoneIndex==MaxDoneIndex){
        DoneList.push_back(DoneInfo(commandtype,layoutindex,aftershape,beforeshape));
        NowDoneIndex++;
        MaxDoneIndex=NowDoneIndex;
    }else if(NowDoneIndex<MaxDoneIndex){
        NowDoneIndex++;
        MaxDoneIndex=NowDoneIndex;
        DoneList[NowDoneIndex]=DoneInfo(commandtype,layoutindex,aftershape,beforeshape);
    }else{
        //Wrong!
        qDebug()<<"DoneList Wrong!!";
    }
 }

 void Model::redo(){
    if(NowDoneIndex>=MaxDoneIndex){
        //Cannot redo
        qDebug()<<"Cannot redo";
    }else{
        NowDoneIndex++;
        //Now Assume that there is no delete in the operation.
        DoneInfo nowInfo=DoneList[NowDoneIndex];
        switch(nowInfo.getcommandtype()){
        case COMMAND::CREATE:
        {
            //redo create
            int insertindex=nowInfo.getlayoutindex();
            vector<shared_ptr<BaseShape>>::iterator it=layouts.list.begin()+insertindex;
            addBaseShape(it,nowInfo.getaftershape());
            //layouts.list.insert(it,nowInfo.getaftershape());
            //layouts.list.push_back(DoneList[NowDoneIndex].getshape());
            Params params;
            params.setType(NOTIFY::UPDATE_IMAGE_ADD);
            params.setInts({(int)insertindex});
            notify(params);
        }
            break;
        case COMMAND::DELETE:{
            //redo create
            int delindex=nowInfo.getlayoutindex();
            vector<shared_ptr<BaseShape>>::iterator it=layouts.list.begin()+delindex;
            layouts.list.erase(it);
            //layouts.list.insert(it,nowInfo.getaftershape());
            //layouts.list.push_back(DoneList[NowDoneIndex].getshape());
            Params params;
            params.setType(NOTIFY::UPDATE_IMAGE_MINUS);
            params.setInts({(int)delindex});
            notify(params);
        }
            break;
        case COMMAND::MODIFY:{
            //redo modify
            int modifyindex=nowInfo.getlayoutindex();
            vector<shared_ptr<BaseShape>>::iterator it=layouts.list.begin()+modifyindex;
            *it=NewBaseShape(nowInfo.getaftershape());
            //layouts.list.insert(it,nowInfo.getaftershape());
            //layouts.list.push_back(DoneList[NowDoneIndex].getshape());
            Params params;
            params.setType(NOTIFY::UPDATE_IMAGE);
            params.setInts({(int)modifyindex});
            notify(params);
        }
            break;
        }


    }
 }

 void Model::undo(){
    if(NowDoneIndex>=0){
        //Now Assume that the insert must be in the last layout.
        DoneInfo nowInfo=DoneList[NowDoneIndex];
        switch(nowInfo.getcommandtype()){
        case COMMAND::CREATE:{
            //Undo Create
            vector<shared_ptr<BaseShape>>::iterator it=layouts.list.begin()+nowInfo.getlayoutindex();
            layouts.list.erase(it);
            NowDoneIndex--;
            Params params;
            params.setType(NOTIFY::UPDATE_IMAGE_MINUS);
            params.setInts({(int)nowInfo.getlayoutindex()});
            notify(params);}
            break;
        case COMMAND::DELETE:{
            //undo delete
            int delindex=nowInfo.getlayoutindex();
            vector<shared_ptr<BaseShape>>::iterator it=layouts.list.begin()+delindex;
            addBaseShape(it,nowInfo.getbeforeshape());
            //layouts.list.insert(it,nowInfo.getaftershape());
            //layouts.list.push_back(DoneList[NowDoneIndex].getshape());
            Params params;
            params.setType(NOTIFY::UPDATE_IMAGE_ADD);
            params.setInts({(int)delindex});
            notify(params);
        }
            break;
        case COMMAND::MODIFY:{
            //undo modify
            int modifyindex=nowInfo.getlayoutindex();
            vector<shared_ptr<BaseShape>>::iterator it=layouts.list.begin()+modifyindex;
            *it=NewBaseShape(nowInfo.getbeforeshape());
            //layouts.list.insert(it,nowInfo.getaftershape());
            //layouts.list.push_back(DoneList[NowDoneIndex].getshape());
            Params params;
            params.setType(NOTIFY::UPDATE_IMAGE);
            params.setInts({(int)modifyindex});
            notify(params);
        }
            break;
        }


    }else{
        qDebug()<<"Cannot undo";
    }
 }
