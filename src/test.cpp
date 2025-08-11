// #include <memory>
// #include <iostream>



// class SomeClass{
//     public:
//         void SomeFunction(int* Num){
//             printf("Value: %d\n", *Num);
//             std::cout << *Num;
//         }

//         SomeClass(){
//             Ptr = std::make_unique<int>(42);
//             SomeFunction(getPointer());
//         }

//         int* getPointer(){
//             return Ptr.get();
//         }

//     private: 
//         std::unique_ptr<int> Ptr;
        
        
// };

// int main(){

//     SomeClass sc;
//     return 0;
// }