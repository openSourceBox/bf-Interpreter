void printErrorMessage(int messageNumber){
	char errorMessages[][256] = {
		"Program terminated successfully.",
		"Max stack size exceeded.",
		"Closing bracket without opening bracket.",
		"Open bracket without closing bracket.",
		"File failed to load.",
		"File size is bigger than supported size.",
		"No filename specified."
	};

	fprintf(stderr, "%s", errorMessages[messageNumber]);
	fprintf(stderr, "\n");
}
