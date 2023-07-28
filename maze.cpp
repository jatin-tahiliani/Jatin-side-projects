/* Your code here! */

#include "maze.h"

#include "dsets.h"
#include "cs225/PNG.h"
#include "cs225/HSLAPixel.h"
#include <queue>
#include <vector>

using namespace std;


SquareMaze::SquareMaze(){

}

bool SquareMaze::canTravel(int x, int y, int dir) const{
    int temp = y*width_+x;
    if(dir==0){
        return rightwall[temp] == false;
    }
    if(dir==1){
        return downwall[temp] == false;
    }
    if(dir==2){
        return x!=0 && (rightwall[temp-1] == false);
    }
    if(dir==3){
        return y!=0 && (downwall[temp-width_]==false);
    }
    return false;
}

cs225::PNG * SquareMaze::drawMaze() const{
   
    int pngwidth = width_*10+1;
    int pngheight = height_*10+1;
    cs225::PNG * png = new cs225::PNG(pngwidth, pngheight);
    for (int i = 10; i < width_*10+1; i++) {
        png->getPixel(i,0) = cs225::HSLAPixel(0,0,0);
    }
    
    for (int j = 0; j < height_*10+1; j++) {
        png->getPixel(0,j) = cs225::HSLAPixel(0,0,0);
    }
    
    for (int y = 0; y < height_; y++) {
        for (int x = 0; x < width_; x++) {
            int temp = y*width_+x;
            
            if (rightwall[temp]) {
                for (int k = 0; k <= 10; k++) {
                    png->getPixel((x+1)*10,y*10+k) = cs225::HSLAPixel(0,0,0);
                }
            }
            
            if (downwall[temp]) {
                for (int k = 0; k <= 10; k++) {
                    png->getPixel(x*10+k,(y+1)*10) = cs225::HSLAPixel(0,0,0);
                }
            }
        }
    }

    return png;
}

cs225::PNG * SquareMaze::drawMazeWithSolution(){
    cs225::PNG * image = drawMaze();
    std::vector<int> drawing = solveMaze();
    int x=5;
    int y=5;
    for (auto dir : drawing){
        if (dir == 0) {
            for(int i=0; i<=10; i++){
                image->getPixel(x + i,y) = cs225::HSLAPixel(0,1,0.5);
            }
            x = x + 10;
        }
        
        if (dir == 1) {
            for(int i=0; i<=10; i++){
                image->getPixel(x ,y+i) = cs225::HSLAPixel(0,1,0.5);
            }
            y = y + 10;
        }
        if (dir == 2) {
            for(int i=0; i<=10; i++){
                image->getPixel(x - i,y) = cs225::HSLAPixel(0,1,0.5);
            }
            x = x - 10;
        }
        if (dir == 3) {
            for(int i=0; i<=10; i++){
                image->getPixel(x ,y-i) = cs225::HSLAPixel(0,1,0.5);
            }
            y = y - 10;
        }
    
    }
    x=x-5;
    y=y+5;
    for(int k=1; k<10; k++){
        image->getPixel((x)+k, (y)) = cs225::HSLAPixel();
    }

    return image;
}	

void SquareMaze::makeMaze(int width, int height){
    width_ = width;
    height_ = height; 
    cout << "width_=" << width_ << ", height: " << height_ << endl;
    int size_ = width_ * height_;
    
   
    rightwall.resize(size_);
    for (int j = 0; j < size_; j++) {
        rightwall[j] = true;
    }
   
    downwall.resize(size_);
    for (int k = 0; k < size_; k++) {
        downwall[k] = true;
    }

    DisjointSets newsets;

    newsets.addelements(size_);

    
    while(newsets.size(0)!=size_){
         int x = rand()%width_;
        int y= rand()%height_;
        
         int temp = (y)*(width_)+ x;
        int rand_dir = rand();
        if(rand_dir%2==0 && x<width_-1 && rightwall[temp]){
            int curr = temp + 1;
            if(newsets.find(temp) != newsets.find(curr)){
                rightwall[temp] = false;
                newsets.setunion(temp, curr);
                
            }
        }
        else if(rand_dir%2==1 && y<height_-1 && downwall[temp]){
            int current = temp + width_;
            if(newsets.find(temp) != newsets.find(current)){
                downwall[temp] = false;
                newsets.setunion(temp,current);
                
            }
        }
    }




}	

void SquareMaze::setWall(int x, int y, int dir, bool exists){
    int temp = y* width_+x; 
    if (dir == 0) { 
        if (exists) { 
            rightwall[temp] = true;
        } 
        else { 
            rightwall[temp] = false;
        }
    } 
    else if (dir == 1) { 
        if (exists) { 
            downwall[temp] = true;
        } 
        else { 
            downwall[temp] = false;
        }
    }
}	


vector< int > SquareMaze::solveMaze(){
    std::vector<bool> isVisited(width_*height_,false);
    std::vector<int> distance(height_*width_,0);
    std::vector<int> p(width_*height_,0);
    std::queue<int> q;
    q.push(0);
    isVisited[0] = true;
    while(q.empty()==false){
        unsigned int temp = q.front();
        q.pop();
        int x = temp%width_;
        int y = temp/width_;
        int tempdistance = distance[temp];

        
        if(temp+1<distance.size() && canTravel(x,y,0) && isVisited[temp+1]==false){
            
            q.push(temp+1);
            isVisited[temp+1] = true;
            distance[temp+1] = tempdistance + 1;
            p[temp+1] = temp;
            
        }
        if(temp+width_<distance.size() && canTravel(x,y,1) && isVisited[temp+width_]==false){
            
            q.push(temp+width_);
            isVisited[temp+width_] = true;
            distance[temp+width_] = tempdistance + 1;
            p[temp+width_] = temp;
            
        }
        if(temp>=1 && canTravel(x,y,2) && isVisited[temp-1]==false){
            
            q.push(temp-1);
            isVisited[temp-1] = true;
            distance[temp-1] = tempdistance + 1;
            p[temp-1] = temp;
            
        }
        if(temp>=(unsigned int)width_ && canTravel(x,y,3) && isVisited[temp-width_]==false){
            
            q.push(temp-width_);
            isVisited[temp-width_] = true;
            distance[temp-width_] = tempdistance + 1;
            p[temp-width_] = temp;
            
        }
    }
    int destination = 0;
    int max = 0;
    for(int i=0; i<width_;i++){
        if(distance[width_*(height_-1)+i]>max){
            destination=width_*(height_-1)+i;
            max = distance[width_*(height_-1)+i];
        }
    }
    vector<int> final;
    int current = destination;
    while(current!=0){
        int before = p[current];
        if(current==before+1){
            final.insert(final.begin(),0);
        }
        else if(current==before+width_){
            final.insert(final.begin(),1);
        }
        else if(current==before-1){
            final.insert(final.begin(),2);
        }
        else if(current==before-width_){
            final.insert(final.begin(),3);
        }
        current = before;
    }
    

return final;
    
}
