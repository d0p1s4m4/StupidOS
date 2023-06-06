/*
 * File: main.pls
 */

/*
 * Procedure: Motd
 * Program entry point
 */
Motd: PROC OPTIONS(MAIN);
	DCL in FILE;
	OPEN FILE(in) TITLE('/etc/motd') INPUT;
	DO ;

	END;
	CLOSE FILE(in);
END;
