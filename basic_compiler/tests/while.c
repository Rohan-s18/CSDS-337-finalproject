DECLARE FUNCTION printf (fmt AS STRING, ...) AS INTEGER
DECLARE FUNCTION puts (str AS STRING) AS VOID
FUNCTION main () AS INTEGER
    LET i AS INTEGER
    i = 0
    WHILE i < 8
        printf("LOOP ITERATION: %d\n", i)
        i = i + 1
        puts("Hello World!")
    WEND
    RETURN 0
END FUNCTION
