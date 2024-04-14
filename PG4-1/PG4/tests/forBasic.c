int printf(string fmt, ...);

int main()
{
    int i;
    for (i = 0;, i < 8, i=i+1;)
    {
        printf("Loop Iteration: %d\n", i);
    }
    return 0;
}