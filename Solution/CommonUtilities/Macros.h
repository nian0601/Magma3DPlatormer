#define PI 3.14159265f

#define MAX(number1,number2) (((number1) > (number2)) ? number1 : number2)
#define MIN(number1,number2) (((number1) > (number2)) ? number2 : number1)

#define TO_RAD(number) (number) * PI / 180
#define TO_DEG(number) (number) * 180 / PI


#define SAFE_DELETE(ptr) delete ptr; ptr = nullptr;
#define SAFE_ARRAY_DELETE(ptr) delete[] ptr; ptr = nullptr;
#define SAFE_RELEASE(ptr) if (ptr) { ptr->Release(); ptr = nullptr;}