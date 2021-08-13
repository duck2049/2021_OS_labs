//s1µÄÇ°×ºÆ¥Åäs2
int myStrcmp(char* s1, char* s2) {
	int m = 0, n = 0;
	while (s1[m] != '\0' && s2[n] != '\0') {
		if (s1[m] != s2[n]) {
			return 0;
		}
		m++;
		n++;
	}
	if (s2[n] == '\0' && (s1[m]=='\0' || s1[m]==' ')) {
		return 1;
	}
	else {
		return 0;
	}
}

//void substr(char* src, char* tar, int head, int len) {
//	for (int i = 0; i < len; i++) {
//		tar[i] = src[i + head];
//	}
//	tar[len] = '\0';
//	return;
//}

int myStrlen(char* s) {
	int i = 0;
	while (s[i] != '\0') {
		i++;
	}
	return i;
}

//s2 copy to s1
void myStrcpy(char* s1, char* s2) {
	int m = 0, n = 0;
	while (s2[n] != '\0') {
		s1[m++] = s2[n++];
	}
	s1[m] = '\0';
}