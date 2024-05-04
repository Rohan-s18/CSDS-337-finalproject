DECLARE FUNCTION printf (fmt AS STRING, ...) AS INTEGER
FUNCTION main () AS INTEGER
    LET i AS INTEGER
    i = 0
    WHILE i < 8
        printf("LOOP ITERATION: %d\n", i = i + 1)
    WEND
    RETURN 0
END FUNCTION
