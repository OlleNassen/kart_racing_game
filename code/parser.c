
static void EatAllWhiteSpace(ini_parser* Parser)
{
	for (;;)
	{
		if (IsWhiteSpace(Parser->At[0]))
		{
			++Parser->At;
		}
		else if (Parser->At[0] == ';')
		{
			while (Parser->At[0] && !IsEndOfLine(Parser->At[0]))
			{
				++Parser->At;
			}
		}
		else
		{
			break;
		}
	}
}

static b32 StringEquals(const char* A, const parser_string* B)
{
    b32 Result = true;
    for (s64 i = 0; i < B->Length; ++i)
    {
        if (A[i] != B->Text[i])
        {
            Result = false;
            break;
        }
    }
    return Result;
}

static b32 Next(ini_parser* Parser)
{
	EatAllWhiteSpace(Parser);
	
	if (Parser->At[0] == '[')
	{
		++Parser->At;
		Parser->Section.Text = Parser->At;
        
		while (Parser->At[0] && Parser->At[0] != ']')
		{
			++Parser->At;
		}
		Parser->Section.Length = Parser->At - Parser->Section.Text;
		++Parser->At;
	}
	
	b32 hasKey = false;
	b32 hasValue = false;
    
	while (!(hasKey && hasValue) && Parser->At[0])
	{
		EatAllWhiteSpace(Parser);
        
		if (Parser->At[0] == '=')
		{
			++Parser->At;
			Parser->Value.Text = Parser->At;
			while (Parser->At[0] && !IsEndOfLine(Parser->At[0]))
			{
				++Parser->At;
			}
			Parser->Value.Length = Parser->At - Parser->Value.Text;
			hasValue = true;
		}
		else
		{
			Parser->Key.Text = Parser->At;
			while (Parser->At[0] && Parser->At[0] != '=')
			{
				++Parser->At;
			}
			Parser->Key.Length = Parser->At - Parser->Key.Text;
			hasKey = true;
		}
	}
    
	return Parser->At[0] != 0 || (hasKey && hasValue);
}

static s64 ToInt64(parser_string* String)
{
    s64 Sign = 1;
    s64 Result = 0;
	for (s64 i = 0; i < String->Length; ++i)
	{
		if (IsNumber(String->Text[i]))
        {
            Result = Result * 10 + (String->Text[i] - '0');
        }
        else if (Result == 0 && String->Text[i] == '-')
        {
            Sign = -1;
        }
        else
        {
            break;
        }
        
	}
    Result *= Sign;
	return Result;
}
