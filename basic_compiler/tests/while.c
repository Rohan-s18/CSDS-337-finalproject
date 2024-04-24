FUNCTION main ()
    LET i AS INTEGER
    i = 0
    WHILE i < 8
        PRINT "Loop Iteration: "; i
        i = i + 1
    WEND
    main = 0
END FUNCTION

PRINT main()
