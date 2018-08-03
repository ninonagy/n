/*
TODO:
- error reporting
    - on loading xml or html file
- add skip variable to parser struct
- add parser.is_eof
- add n_alloc
https://github.com/google/gumbo-parser
*/

#ifndef N_HTML_PARSER_H
#define N_HTML_PARSER_H

#include <malloc.h>
#define n_alloc(size) malloc(size)

typedef unsigned char n_uint8;
typedef unsigned int  n_uint32;

#include <math.h> // for pow
#include <stdlib.h> // for atoi

#include "n_string.h"
#include "n_node.h"
#define ENABLE_FILE
#include "n_utils.h"

typedef n_string String;
typedef n_list* List;

static int empty_elements_count = 18;
static const char *empty_elements[] = 
{
    "area",
    "base",
    "br",
    "col",
    "colgroup",
    "command",
    "embed",
    "hr",
    "img",
    "image",
    "input",
    "keygen",
    "link",
    "meta",
    "param",
    "source",
    "track",
    "wbr"
};

static const char *named_entities[] = 
{
    // https://en.wikipedia.org/wiki/List_of_XML_and_HTML_character_entity_references#Predefined_entities_in_XML
    "quot",     "\"",
    "amp",      "&",
    "apos",     "'",
    "lt",       "<",
    "gt",       ">",
    "nbsp",     " ",
    "iexcl",	"¡",
    "cent",		"¢",
    "pound",    "£",
    "curren",   "¤",
    "yen",      "¥",
    "brvbar",   "¦",
    "sect",     "§",
    "uml",      "¨",
    "copy",		"©",
    "ordf",		"ª",
    "laquo",	"«",
    "not",		"¬",
    "shy",		" ",
    "reg",		"®",
    "macr",		"¯",
    "deg",		"°",
    "plusmn",	"±",
    "sup2",		"²",
    "sup3",		"³",
    "acute",	"´",
    "micro",	"µ",
    "para",		"¶",
    "middot",	"·",
    "cedil",	"¸",
    "sup1",		"¹",
    "ordm",		"º",
    "raquo",	"»",
    "frac14",	"¼",
    "frac12",	"½",
    "frac34",	"¾",
    "iquest",	"¿",
    "Agrave",	"À",
    "Aacute",	"Á",
    "Acirc",	"Â",
    "Atilde",	"Ã",
    "Auml",		"Ä",
    "Aring",	"Å",
    "AElig",	"Æ",
    "Ccedil",	"Ç",
    "Egrave",	"È",
    "Eacute",	"É",
    "Ecirc",	"Ê",
    "Euml",		"Ë",
    "Igrave",	"Ì",
    "Iacute",	"Í",
    "Icirc",	"Î",
    "Iuml",		"Ï",
    "ETH",		"Ð",
    "Ntilde",	"Ñ",
    "Ograve",	"Ò",
    "Oacute",	"Ó",
    "Ocirc",	"Ô",
    "Otilde",	"Õ",
    "Ouml",		"Ö",
    "times",	"×",
    "Oslash",	"Ø",
    "Ugrave",	"Ù",
    "Uacute",	"Ú",
    "Ucirc",	"Û",
    "Uuml",		"Ü",
    "Yacute",	"Ý",
    "THORN",	"Þ",
    "szlig",	"ß",
    "agrave",	"à",
    "aacute",	"á",
    "acirc",	"â",
    "atilde",	"ã",
    "auml",		"ä",
    "aring",	"å",
    "aelig",	"æ",
    "ccedil",	"ç",
    "egrave",	"è",
    "eacute",	"é",
    "ecirc",	"ê",
    "euml",		"ë",
    "igrave",	"ì",
    "iacute",	"í",
    "icirc",	"î",
    "iuml",		"ï",
    "eth",		"ð",
    "ntilde",	"ñ",
    "ograve",	"ò",
    "oacute",	"ó",
    "ocirc",	"ô",
    "otilde",	"õ",
    "ouml",		"ö",
    "divide",	"÷",
    "oslash",	"ø",
    "ugrave",	"ù",
    "uacute",	"ú",
    "ucirc",	"û",
    "uuml",     "ü",
    "yacute",	"ý",
    "thorn",	"þ",
    "yuml",		"ÿ",
    "OElig",	"Œ",
    "oelig",	"œ",
    "Scaron",	"Š",
    "scaron",	"š",
    "Yuml",		"Ÿ",
    "fnof",		"ƒ",
    "circ",		"ˆ",
    "tilde",	"˜",
    "Alpha",	"Α",
    "Beta",		"Β",
    "Gamma",	"Γ",
    "Delta",	"Δ",
    "Epsilon",	"Ε",
    "Zeta",		"Ζ",
    "Eta",		"Η",
    "Theta",	"Θ",
    "Iota",		"Ι",
    "Kappa",	"Κ",
    "Lambda",	"Λ",
    "Mu",		"Μ",
    "Nu",		"Ν",
    "Xi",		"Ξ",
    "Omicron",	"Ο",
    "Pi",		"Π",
    "Rho",		"Ρ",
    "Sigma",	"Σ",
    "Tau",		"Τ",
    "Upsilon",	"Υ",
    "Phi",		"Φ",
    "Chi",		"Χ",
    "Psi",		"Ψ",
    "Omega",	"Ω",
    "alpha",	"α",
    "beta",		"β",
    "gamma",	"γ",
    "delta",	"δ",
    "epsilon",	"ε",
    "zeta",		"ζ",
    "eta",		"η",
    "theta",	"θ",
    "iota",		"ι",
    "kappa",	"κ",
    "lambda",	"λ",
    "mu",		"μ",
    "nu",		"ν",
    "xi",		"ξ",
    "omicron",  "ο",
    "pi",		"π",
    "rho",		"ρ",
    "sigmaf",	"ς",
    "sigma",	"σ",
    "tau",		"τ",
    "upsilon",	"υ",
    "phi",		"φ",
    "chi",		"χ",
    "psi",		"ψ",
    "omega",	"ω",
    "thetasym",	"ϑ",
    "upsih",	"ϒ",
    "piv",		"ϖ",
    "ensp",		"\xE2\x80\x82",
    "emsp",		"\xE2\x80\x83",
    "thinsp",	"\xE2\x80\x89",
    "zwnj",		"\xE2\x80\x8C",
    "zwj",		"\xE2\x80\x8D",
    "lrm",		"\xE2\x80\x8E",
    "rlm",		"\xE2\x80\x8F",
    "ndash",	"–",
    "mdash",	"—",
    "lsquo",	"‘",
    "rsquo",	"’",
    "sbquo",	"‚",
    "ldquo",	"“",
    "rdquo",	"”",
    "bdquo",	"„",
    "dagger",	"†",
    "Dagger",	"‡",
    "bull",		"•",
    "hellip",	"…",
    "permil",	"‰",
    "prime",	"′",
    "Prime",	"″",
    "lsaquo",	"‹",
    "rsaquo",	"›",
    "oline",	"‾",
    "frasl",	"⁄",
    "euro",		"€",
    "image",	"ℑ",
    "weierp",	"℘",
    "real",		"ℜ",
    "trade",	"™",
    "alefsym",	"ℵ",
    "larr",		"←",
    "uarr",		"↑",
    "rarr",		"→",
    "darr",		"↓",
    "harr",		"↔",
    "crarr",	"↵",
    "lArr",		"⇐",
    "uArr",		"⇑",
    "rArr",		"⇒",
    "dArr",		"⇓",
    "hArr",		"⇔",
    "forall",	"∀",
    "part",		"∂",
    "exist",	"∃",
    "empty",	"∅",
    "nabla",	"∇",
    "isin",     "∈",
    "notin",	"∉",
    "ni",       "∋",
    "prod",     "∏",
    "sum",      "∑",
    "minus",	"−",
    "lowast",	"∗",
    "radic",	"√",
    "prop",     "∝",
    "infin",	"∞",
    "ang",      "∠",
    "and",      "∧",
    "or",       "∨",
    "cap",      "∩",
    "cup",      "∪",
    "int",      "∫",
    "there4",	"∴",
    "sim",      "∼",
    "cong",     "≅",
    "asymp",	"≈",
    "ne",       "≠",
    "equiv",	"≡",
    "le",       "≤",
    "ge",       "≥",
    "sub",      "⊂",
    "sup",      "⊃",
    "nsub",     "⊄",
    "sube",     "⊆",
    "supe",     "⊇",
    "oplus",	"⊕",
    "otimes",	"⊗",
    "perp",     "⊥",
    "sdot",     "⋅",
    "lceil",	"⌈",
    "rceil",	"⌉",
    "lfloor",	"⌊",
    "rfloor",	"⌋",
    "lang",     "〈",
    "rang",     "〉",
    "loz",      "◊",
    "spades",	"♠",
    "clubs",	"♣",
    "hearts",	"♥",
    "diams",	"♦",
};

inline n_uint32 hex_value(char c)
{
    n_uint32 result = 0;
    switch (c)
    {
        case '0': result = 0; break;
        case '1': result = 1; break;
        case '2': result = 2; break;
        case '3': result = 3; break;
        case '4': result = 4; break;
        case '5': result = 5; break;
        case '6': result = 6; break;
        case '7': result = 7; break;
        case '8': result = 8; break;
        case '9': result = 9; break;
        case 'A': result = 10; break;
        case 'B': result = 11; break;
        case 'C': result = 12; break;
        case 'D': result = 13; break;
        case 'E': result = 14; break;
        case 'F': result = 15; break;
        default: assert(!"Invalid char");
    }
    return result;
}

static n_uint32 hex_to_dec(char *c)
{
    int size = strlen(c);
    char at;
    n_uint32 value = 0;
    assert(size != 1);
    if(size == 2)
    {
        n_uint32 at0 = hex_value('0');
        n_uint32 at1 = hex_value('0');
        n_uint32 at2 = hex_value(*(c + 0));
        n_uint32 at3 = hex_value(*(c + 1));
        value = (at0 * pow(16, 3)) + (at1 * pow(16, 2)) + 
                (at2 * pow(16, 1)) + (at3 * 1);
    }
    else if(size == 3)
    {
        n_uint32 at0 = hex_value(*(c + 0));
        n_uint32 at1 = hex_value(*(c + 1));
        n_uint32 at2 = hex_value(*(c + 2));
        value = (at0 * pow(16, 2)) + (at1 * pow(16, 1)) + (at2 * 1);
    }
    else if(size == 4)
    {
        n_uint32 at0 = hex_value(*(c + 0));
        n_uint32 at1 = hex_value(*(c + 1));
        n_uint32 at2 = hex_value(*(c + 2));
        n_uint32 at3 = hex_value(*(c + 3));
        value = (at0 * pow(16, 3)) + (at1 * pow(16, 2)) + 
                (at2 * pow(16, 1)) + (at3 * 1);
    }
    
    return value;
}

#define array_count(array) (sizeof(array) / sizeof((array)[0]))

static char *
get_named_entity(char *name)
{
    char *found = 0;
    
    for(int i = 0;
        i < array_count(named_entities);
        i += 2)
    {
        if(strcmp(named_entities[i], name) == 0)
        {
            found = (char *)named_entities[i + 1];
            break;
        }
    }
    
    return found;
}

static String
get_unicode_char_utf8(n_uint32 cp)
{
    n_uint8 *bytes = 0;
    if(cp <= 0x007Ful)
    {
        bytes = (n_uint8 *)n_alloc(1);
        bytes[0] = (n_uint8)cp;
        return new_string_len((char *)bytes, 1);
    }
    if(cp <= 0x07FFul)
    {
        bytes = (n_uint8 *)n_alloc(2);
        bytes[1] = (n_uint8)((2 << 6) | (cp & 0x3F));
        bytes[0] = (n_uint8)((6 << 5) | (cp >> 6));
        return new_string_len((char *)bytes, 2);
    }
    if(cp <= 0xFFFFul)
    {
        bytes = (n_uint8 *)n_alloc(3);
        bytes[2] = (n_uint8)(( 2 << 6) | ( cp       & 0x3F));
        bytes[1] = (n_uint8)(( 2 << 6) | ((cp >> 6) & 0x3F));
        bytes[0] = (n_uint8)((14 << 4) |  (cp >> 12));
        return new_string_len((char *)bytes, 3);
    }
    if(cp <= 0x10FFFFul)
    {
        bytes = (n_uint8 *)n_alloc(4);
        bytes[3] = (n_uint8)(( 2 << 6) | ( cp        & 0x3F));
        bytes[2] = (n_uint8)(( 2 << 6) | ((cp >>  6) & 0x3F));
        bytes[1] = (n_uint8)(( 2 << 6) | ((cp >> 12) & 0x3F));
        bytes[0] = (n_uint8)((30 << 3) |  (cp >> 18));
        return new_string_len((char *)bytes, 4);
    }
}

static char *
html_decode_entities_utf8(char *cstr)
{
    char *at = cstr;
    n_string content = new_string("");
    while(!string_end(at))
    {
        content = add_string(content, string_consume_while(&at, "&"));
        
        // TODO: Merge hexadecimal and decimal in one if statement?
        // hexadecimal code
        if(string_starts_with(at, "&#x") ||
           string_starts_with(at, "&#X"))
        {
            assert(*at++ == '&');
            assert(*at++ == '#');
            assert(*at++ == 'x');
            n_string hex_code = string_consume_while(&at, ";");
            assert(*at++ == ';');
            // convert hexadecimal to decimal
            n_uint32 code_point = hex_to_dec(hex_code);
            content = add_string(content, get_unicode_char_utf8(code_point));
        }
        // decimal code
        else if(string_starts_with(at, "&#"))
        {
            assert(*at++ == '&');
            assert(*at++ == '#');
            n_string code = string_consume_while(&at, ";");
            assert(*at++ == ';');
            // NOTE: Converts string to unsigned integer
            // http://en.cppreference.com/w/cpp/string/basic_string/stol
            n_uint32 code_point = atoi(code);
            content = add_string(content, get_unicode_char_utf8(code_point));
        }
        // named entity
        else if(*at == '&')
        {
            assert(*at++ == '&');
            n_string name = string_consume_while(&at, ";");
            char *entity = get_named_entity(name);
            if(entity)
            {
                assert(*at++ == ';');
                content = add_string(content, new_string_len(entity, 1));
            }
            //assert(entity);
        }
    }
    
    return content;
}

static bool is_empty_element(n_string str)
{
    bool result = false;
        
    for(int i = 0;
        i < empty_elements_count;
        ++i)
    {
        // TODO: ???
        // n_string element_name = new_string(str, string_len(str));
        if(strcmp(empty_elements[i], str) == 0)
        {
            result = true;
            break;
        }
    }
    
    return result;
}

// @parser

typedef enum
{
    parser_type_html,
    parser_type_xml,
} parser_type;

typedef struct
{
    parser_type type;
    char *input_stream;
    char *eof;
    char *at;

    // char *string_storage;
    // size_t string_storage_len;

#if 1
    bool is_end;
    bool invalid_file;
    n_string last_element;
#endif
} n_parser;

static n_parser parser;

static void
parser_init(n_parser *parser,
            const char *input_stream,
            const char *stream_end)
            //char *string_storage,
            //int storage_length)
{
    parser->input_stream = (char *)input_stream;
    parser->at = (char *)input_stream;
    parser->eof = (char *)stream_end;
    //parser->string_storage = string_storage;
    //parser->string_storage_len = storage_length;
}

inline char next_char() { return *parser.at; }
inline char last_char() { return *(parser.at-1); }
inline char consume_char() { return (*parser.at++); }

inline String parser_consume_while(const char *cstr)
{
    return string_consume_while(&parser.at, cstr);
}

static char *consume_while_char(char a)
{
    char *string = parser.at;
    int str_len = 0;
    while(next_char() != a)
    {
        consume_char();
        ++str_len;
    }

    return new_string_len(string, str_len);
}

inline void consume_whitespace()
{
    while(parser.at != parser.eof &&
          is_white(next_char()))
    {
        consume_char();
    }
}

enum
{
    node_type_element,
    node_type_attrib,
    node_type_text,
};

// @node

union node_union;
typedef union node_union node_union;
typedef node_union* Node;

union node_union
{
    struct // element node
    {
        String tag_name;
        Node attribs;
        Node child;
    };
    struct // attribute node
    {
        String name;
        String value;
    };  
    struct // text node
    {
        String text;
        int is_cdata; // TODO: bool32
    };
};

typedef enum {
    token_unknown,

    token_start_tag,
    token_end_tag,
    token_cdata,
    token_eof,

    token_count,
} TokenType;

TokenType get_token()
{
    consume_whitespace();
}

static List parse_nodes(bool);
static Node parse_node(bool);
static Node parse_element();
static Node parse_text(bool);
static String parse_tag_name();
static String peek_tag_name();
static List parse_attributes();
static Node parse_attrib();
static String parse_attrib_value();
static void parse_comment();

static Node
element(String name, List attribs, List children)
{
    Node result = new_node_type(node_type_element, node_union);
    if(result == 0) return 0;
    
    result->tag_name = name;
    result->attribs = (Node)get_first_node(attribs);
    result->child = (Node)get_first_node(children);
    
    return result;
}

// TODO: Add skip to parser struct?
static List
parse_nodes(bool skip)
{
    List nodes = new_list();

    while(true)
    {
        assert(parser.at);
        consume_whitespace();
        
        if(parser.at == parser.eof)
        {
            parser.is_end = true;
            break;
        }
        
        if(string_starts_with(parser.at, "</")) break;

        if(string_starts_with(parser.at, "<!--"))
        {
            parse_comment();
        }
        else
        {
            if(nodes->first_node && parser.last_element)
            {
                Node node = (Node)get_node_index(nodes, 1);
                if(strings_are_equal(node->tag_name,
                                     parser.last_element))
                {
                    parser.is_end = true;
                }
            }
            
            if(parser.is_end) return nodes;
            
            insert_back(nodes, parse_node(skip));
        }
    }

    return nodes;
}

static Node parse_node(bool skip)
{
    Node node = 0;

    if((next_char() == '<' && !skip) &&
       !string_starts_with(parser.at, "<![CDATA["))
    {
        node = parse_element();
    }
    else
    {
        node = parse_text(skip);
    }

#if 0
    int node_type = get_node_type(node);
    if (node_type == node_type_element) {
        print("%s\n", node->tag_name); }
    else if (node_type == node_type_text) {
        print("%s\n", node->text); }
    else if (node_type == node_type_attrib) {
        print("%s\n", node->name); }
#endif

    return node;
}

static Node parse_element()
{
    // Opening tag
    assert(consume_char() == '<');
    String tag_name = parse_tag_name();
    string_to_lower(tag_name);
    
    if(parser.type == parser_type_xml) {
        if(strings_are_equal(tag_name, "html") ||
           string_starts_with(parser.at, "!DOCTYPE")) {
            parser.invalid_file = true;
            parser.is_end = true;
            return 0;
        }
    }
    
    List attribs = parse_attributes();
    
    bool is_empty = false;
    if(string_starts_with(parser.at, "/>"))
    {
        assert(consume_char() == '/');
        assert(consume_char() == '>');
        is_empty = true;
    }
    else
    {
        assert(consume_char() == '>');
    }
    
    List children = new_list();
    if(parser.type == parser_type_html)
    {
        // If is empty element having closing tag.
        String tag_end = add_string(new_string("</"), tag_name);
        tag_end = add_string(tag_end, new_string(">"));

        if(!is_empty_element(tag_name) ||
           string_starts_with(parser.at, tag_end))
        {
            bool skip = false;
            if(strings_are_equal(tag_name, "script"))
                skip = true;
            
            // Contents
            children = parse_nodes(skip);
            
            if(!parser.is_end)
            {
                // Closing tag
                assert(consume_char() == '<');
                assert(consume_char() == '/');
                String end_tag = parse_tag_name();
                string_to_lower(end_tag);
                assert(strings_are_equal(end_tag, tag_name));
                assert(consume_char() == '>');
            }
        }
    }
    else if(parser.type == parser_type_xml)
    {
        if(!is_empty)
        {
            children = parse_nodes(0);
        
            // Closing tag
            assert(consume_char() == '<');
            assert(consume_char() == '/');
            
            String end_tag = parse_tag_name();
            string_to_lower(end_tag);

            assert(strings_are_equal(end_tag, tag_name));
            assert(consume_char() == '>');
        }
    }
    
    return element(tag_name, attribs, children);
}

static String parse_tag_name()
{
    String result = 0;

    char *string = parser.at;
    char at = next_char();

    int str_len = 0;
    while(parser.at != parser.eof &&
          ((at >= 'a' && at <= 'z') ||
          (at >= 'A' && at <= 'Z') ||
          (at >= '0' && at <= '9') ||
          (at == '-' || at == ':')))
    {
        consume_char();
        at = next_char();
        ++str_len;
    }

    result = new_string_len(string, str_len);

    return result;
}

// TODO: Is this necessery?
static String peek_tag_name()
{
    String result = 0;

    char *string = parser.at;
    char at = next_char();

    int string_length = 0;
    while(!string_end(parser.at) &&
          ((at >= 'a' && at <= 'z') ||
          (at >= 'A' && at <= 'Z') ||
          (at >= '0' && at <= '9') ||
          (at == '-')))
    {
        at = next_char();
        ++string_length;
    }

    result = new_string_len(string, string_length);

    return result;
}

static List parse_attributes()
{
    List attributes = new_list();
    
    while(true)
    {
        consume_whitespace();
        if(string_starts_with(parser.at, "/>"))
        {
            //assert(consume_char() == '/');
            break;
        }
        else if(next_char() == '>')
        {
            break;
        }

        Node attrib = parse_attrib();
        insert_back(attributes, attrib);
    }

    return attributes;
}

static Node parse_attrib()
{
    Node node = new_node_type(node_type_attrib, node_union);

    node->name = parse_tag_name();

    if(next_char() == '=')
    {
        assert(consume_char() == '=');
        consume_whitespace();
        node->value = parse_attrib_value();
    }
    
    return node;
}

static String parse_attrib_value()
{
    String value = 0;

    if(string_starts_with(parser.at, "\"\""))
    {
        assert(consume_char() == '"');
        assert(consume_char() == '"');
        // value = new_string("");
    }
    else if(next_char() == '"' ||
            next_char() == '\'')
    {
        //<title>width="32 height="32" border="0" rel="shortcut icon"></title>
        //<title>width="32 height="32" border="0" alt="Facebook ></title>
        char open_quote = consume_char();
        assert(open_quote == '"' || open_quote == '\'');
        value = consume_while_char(open_quote);
        assert(consume_char() == open_quote);
        
        /*int counter_point = 0;
        int counter = 0;
        char expected_char = open_quote;
        char *temp = parser.at;
        for(;;)
        {
            if(*temp == ' ' ||
               *temp == '>' ||
               string_starts_with(temp, "/>") ||
               *temp == expected_char ||
               *temp == open_quote)
            {
                if(*temp == ' ')
                {
                    counter_point = counter;
                    expected_char = '=';
                    if(*(temp+1) == '>' ||
                       string_starts_with(temp+1, "/>"))
                    {
                        value = consume_while(*temp);
                        assert(consume_char() == *temp);
                        break;
                    }
                }
                else if(*temp == '=')
                {
                    int dis = counter - counter_point;
                    temp -= dis;
                    value = consume_while(*temp);
                    assert(consume_char() == *temp);
                    break;
                }
                else if(*temp == open_quote)
                {
                    value = consume_while(open_quote);
                    assert(consume_char() == open_quote);
                    break;
                }
            }
            ++counter;
            ++temp;
        }*/
    }
    else
    {
        char *string = parser.at;
        int len = 0;
        while(!is_white(*parser.at++))
        {
            ++len;
            if(next_char() == '>') break;
        }

        value = new_string_len(string, len);
    }

    return value;
}

static Node parse_text(bool skip)
{
    Node result = new_node_type(node_type_text, node_union);

    if(string_starts_with(parser.at, "<![CDATA["))
    {
        // TODO: Function!
        assert(consume_char() == '<');
        assert(consume_char() == '!');
        assert(consume_char() == '[');
        assert(consume_char() == 'C');
        assert(consume_char() == 'D');
        assert(consume_char() == 'A');
        assert(consume_char() == 'T');
        assert(consume_char() == 'A');
        assert(consume_char() == '[');
        
        result->text = parser_consume_while("]]>");
        
        assert(consume_char() == ']');
        assert(consume_char() == ']');
        assert(consume_char() == '>');
        
        result->is_cdata = true;
    }
    else if(!skip)
    {
        result->text = parser_consume_while("<");
    }
    else
    {
        // TODO: I think we don't need this anymore because
        //       script element has a CDATA (NO IT DOESN'T!).
        while(true) {
            // TODO: It is very rare to be in this branch of code!
            //       We don't need data from here, it just consumes
            //       chars.
            char *text = parser_consume_while("<");
            //String text = consume_while('<');
            //if(result->text.length == 0) result->text = text;
            //else result->text = result->text + text;
            if(string_starts_with(parser.at, "</script>")) {
                break;
            }

            ++parser.at;
        }
    }

    return result;
}

static void parse_comment()
{
    assert(consume_char() == '<');
    assert(consume_char() == '!');
    assert(consume_char() == '-');
    assert(consume_char() == '-');

    while(true)
    {
        parser_consume_while("-");
        if(string_starts_with(parser.at, "-->"))
        {
            assert(consume_char() == '-');
            assert(consume_char() == '-');
            assert(consume_char() == '>');
            break;
        }
        ++parser.at;
    }
}

static void parse_comments()
{
    while(true)
    {
        consume_whitespace();
        if(string_starts_with(parser.at, "<!--")) {
            parse_comment();
        }
        else break;
    }
}

static void parse_doctype()
{
    while(is_white(next_char())) ++parser.at;
    if(string_starts_with(parser.at, "<!"))
    {
        assert(consume_char() == '<');
        assert(consume_char() == '!');
        
        String name = parser_consume_while(" ");
        string_to_lower(name);
        
        if(strings_are_equal(name, "doctype"))
        {
            consume_whitespace();
            String h = new_string_len(parser.at, 4);
            string_to_lower(h);
            if(strings_are_equal(h, "html"))
            {
                parser.at += 4;
                //result = true;
                while(*parser.at++ != '>') {}
            }
        }
    }
}

static List parse_html()
{
    assert(parser.at);
    
    parse_comments();
    parse_doctype();
    
    parser.type = parser_type_html;
    parser.is_end = false;
    parser.invalid_file = false;
    parser.last_element = 0;
    
    List nodes = parse_nodes(0);

    return nodes;
}

static void parse_xml_prolog()
{
    parse_comments();
    while(string_starts_with(parser.at, "<?"))
    {
        assert(consume_char() == '<');
        assert(consume_char() == '?');
        consume_while_char(' ');

        while(!string_starts_with(parser.at, "?>"))
        {
            consume_whitespace();
            // TODO: Capture version and encoding
            parse_attrib();
        }

        assert(consume_char() == '?');
        assert(consume_char() == '>');
        
        consume_whitespace();
    }
}

static List parse_xml()
{
    assert(parser.at);
        
    parse_xml_prolog();
    
    parser.type = parser_type_xml;
    parser.is_end = false;
    parser.invalid_file = false;
    parser.last_element = 0;

    List nodes = parse_nodes(0);
    
    return nodes;
}

static String
parse_description_text(Node text_node)
{
    String content = text_node->text;

    content = html_decode_entities_utf8(content);
    
    // TODO: Remove <i>, <b> tags from content
    if(text_node->is_cdata || 
       string_starts_with(content, "<"))
    {
        n_parser tmp_parser = parser;
        parser_init(&parser, content, content + string_len(content));
        
        List list = parse_html();
        
        // Gets text in <p> not in <image> exc.
        for_each_node(list->first_node, Node)
        {
            if(get_node_type(it) == node_type_text)
            {
                content = it->text;
                break;
            }
        }
        
        parser = tmp_parser;
        
        content = html_decode_entities_utf8(content);
    }
    
    // print(content);
    
    // Remove newline and return
    string_remove_chars(content, "\n\r");
    if(string_at(content, 0) == ' ')
    {
        string_erase_at(content, 0);
    }
    
    // Remove brackets
    while(true)
    {
        int found_left = string_find(content, "(");
        if(found_left != npos) {
            int found_right = string_find_from(content, ")", found_left);
            if(found_right != npos)
            {
                string_erase_len(content, found_left - 1,
                                 (found_right - found_left) + 2);
            }
            else break;
        }
        else break;
    }
    
    // Reads only first sentence!
    // if is number after dot then is some date, we skip that
    const char *end_chars = ".!?";
    int found = 0;
    while(true)
    {
        found = string_find_chars(content, end_chars, found);
        if(found != npos)
        {
            int after_punc = found + 1;
            if(after_punc < string_len(content))
            {
                char number = string_at(content, after_punc + 1);
                if(is_number(number)) {
                    ++found;
                    continue;
                }
            }

            int erase_length = string_len(content) - after_punc;
            string_erase_len(content, after_punc, erase_length);
            
            break;
        }
        else break;
    }
    
    return content;
}

static String
get_text(Node node)
{
    return ((node->child) ? node->child->text : 0);
}

// NOTE: Get first attribute node with given name
static Node
get_attrib(Node node, const char *name)
{
    Node result = 0;

    for_each_node(node->attribs, Node)
    {
        if(strings_are_equal(it->name, name))
        {
            result = it;
            //result.name = it->name;
            //result.value = it->value;
        }
    }

    return result;
}

static String
get_attrib_value(Node node, const char *name)
{
    String result = 0;
    
    Node attrib_node = get_attrib(node, name);
    if(attrib_node)
    {
        result = attrib_node->value;
    }
    
    return result;
}

// NOTE: Get first child node with given name.
static Node
get_child(Node parent_node, const char *name)
{
    Node result = 0;
    Node child = parent_node->child;

    while(child)
    {
        if (get_node_type(child) == node_type_element && 
            strings_are_equal(child->tag_name, name))
        {
            result = child;
            break;
        }

        child = (Node)get_next_node(child);
    }

    return result;
}

static String
get_child_text(Node parent_node, const char *name)
{
    Node child = get_child(parent_node, name);
    if(child == 0) return 0;

    return get_text(child);
}

// NOTE: Get all the direct child nodes of an element
static List 
get_children(Node parent_node)
{
    List children = new_list();
    children->first_node = parent_node->child;

    return children;
}

// NOTE: Get all the direct child nodes with a given name
static List 
get_children_name(Node parent_node, const char *name)
{
    List children = new_list();
    Node child = parent_node->child;

    while(child)
    {
        if(get_node_type(child) == node_type_element &&
           strings_are_equal(child->tag_name, name))
        {
            Node node = copy_node(child, node_union);
            N_NODE_HEADER(node)->next = 0;
            insert_back(children, node);
        }

        child = (Node)get_next_node(child);
    }

    return children;
}

static List
get_elements_tag_name(List list, const char *name)
{
    List result = new_list();

    Node parent_node = (Node)list->first_node;
    
    while(parent_node)
    {
        if(strings_are_equal(parent_node->tag_name, name))
        {
            Node node = copy_node(parent_node, node_union);
            N_NODE_HEADER(node)->next = 0;
            insert_back(result, node);
        }

        // NOTE: Find child elements
        if(parent_node->child)
        {
            if(get_node_type(parent_node->child) == node_type_element)
            {
                List children = new_list_node(parent_node->child);
                children = get_elements_tag_name(children, name);
                if(children->first_node)
                {
                    // TODO: function for list merging?
                    Node node = copy_node(children->first_node, node_union);
                    insert_back(result, node);
                }
            }
        }
        
        parent_node = (Node)get_next_node(parent_node);
    }

    return result;
}

static Node
get_element_tag_name(List list, const char *name)
{
    // TODO: This might be slow?
    List result = get_elements_tag_name(list, name);
    return (Node)result->first_node;
}

// TODO
static List
get_elements_class_name(List list, const char *name)
{
    List result = new_list();

    Node parent_node = (Node)list->first_node;
    
    while(parent_node)
    {
        String class_name = get_attrib_value(parent_node, "class");
        if(class_name)
        {
            if(strings_are_equal(class_name, name))
            {
                Node node = copy_node(parent_node, node_union);
                N_NODE_HEADER(node)->next = 0;
                insert_back(result, node);
            }
        }

        // NOTE: Find child elements
        if(parent_node->child)
        {
            if(get_node_type(parent_node->child) == node_type_element)
            {
                List children = new_list_node(parent_node->child);
                children = get_elements_class_name(children, name);
                if(children->first_node)
                {
                    // TODO: function for list merging?
                    Node node = copy_node(children->first_node, node_union);
                    insert_back(result, node);
                }
            }
        }
        
        parent_node = (Node)get_next_node(parent_node);
    }

    return result;
}

#endif // N_HTML_PARSER_H