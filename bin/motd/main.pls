/*
 * File: main.pls
 */

/*
 * Procedure: main
 *
 * Program entry point.
 */


PROC main;
	DCL in AS File;
	DCL line AS String;
	DCL now AS DateTime;

	now = [[DateTime allocate] now];
	IF now == NIL THEN exit(1);
	in = [[File allocate] initOpen:"/etc/motd"];
	IF in == NIL THEN exit(1);

	WHILE [in readLine:line] != EOF;
		print(line);
	END;

	[in release];
	[now release];
END;
