int printf(string fmt, ...);

float add(int a, float b)
{
    return a + b;
}

int main()
{
    printf("%f\n", add(3, 7.0));
    return 0;
}