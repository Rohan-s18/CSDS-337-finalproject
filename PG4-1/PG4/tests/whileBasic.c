int printf(string fmt, ...);

int main()
{
    int i;
    i = 0;
    while (i < 8)
    {
        printf("Loop Iteration: %d\n", i);
        i = i + 1;
    }
    return 0;
}