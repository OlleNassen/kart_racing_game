typedef struct parser_string
{
	s64 Length;
	const char* Text;
} parser_string;

typedef struct ini_parser
{
	const char* At;
	parser_string Section;
	parser_string Key;
	parser_string Value;
} ini_parser;

inline b32 IsEndOfLine(char C)
{
	return ((C == '\n') || (C == '\r'));
}

inline b32 IsWhiteSpace(char C)
{
	return ((C == ' ') || (C == '\t') || IsEndOfLine(C));
}


inline b32 IsAlpha(char C)
{
	return ((C >= 'a') && (C <= 'z')) || ((C >= 'A') && (C <= 'Z'));
}

inline b32 IsNumber(char C)
{
	return ((C >= '0') && (C <= '9'));
}

static void EatAllWhiteSpace(ini_parser* Parser);
static b32 StringEquals(const char* A, const parser_string* B);
static b32 Next(ini_parser* Parser);
static s64 ToInt64(parser_string* String);