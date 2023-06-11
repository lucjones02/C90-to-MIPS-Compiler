int f(){
    int x=5;
    for(int i=0; i<10; i++){
        if(x==7){
            continue;
        }
        x = x+1;
    }
    return x;
}