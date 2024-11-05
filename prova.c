int main(){
    int a  = 5;
    if(a == 7)
        a = 6;
    else if(a == 5)
        a = 8;
    else 
        a = 6;
    int b = 5;
    if(a == 6)
        b = 7;
    return a + b;
}