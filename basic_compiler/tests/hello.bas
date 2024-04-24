DECLARE FUNCTION puts (str AS STRING)

FUNCTION puts (str AS STRING)
    PRINT str
END FUNCTION

FUNCTION main ()
    CALL puts("Hello World!")
    main = 0
END FUNCTION

PRINT main()
