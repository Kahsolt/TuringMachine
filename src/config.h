#ifndef CONFIG_H
#define CONFIG_H

/* User Settings */

// parser
#define LINE_BUFFER_SIZE  10240

// definition item defaults
#define TM_DEFAULT_B      '_'
#define TM_DEFAULT_q      "init"
#define TM_DEFAULT_F      "halt"

// definition syntax
#define COMMENT_MARK      ';'
#define WILDCARD_MARK     '*'
#define ITEM_SEPERATOR    ','

// TM hardware limits
#define LIMIT_ENABLE      true
#define LIMIT_TIME        3000
#define LIMIT_SPACE       100

// file devices
#define DEFINITION_FILENAME "test.tm"
#define INPUT_FILENAME      "input.txt"
#define OUTPUT_FILENAME     "result.txt"
#define DEBUG_FILENAME      "console.txt"

// runtime cmdopts defaults
#define OPT_DEFAULT_LOGLEVEL SIMPLE
#define OPT_DEFAULT_SAVE     true

// console print typesettings
#define BANNER_WIDTH      78
#define BANNER_SECTOR     '='
#define BANNER_SEPERATOR  '-'

/* OS Specifics */
#ifdef _WIN32
  #define PATH_SEPERATOR '\\'
  #define NULL_DEVICE "NUL"
#elif defined(__linux__) || defined(__unix__)
  #define PATH_SEPERATOR '/'
  #define NULL_DEVICE "/dev/null"
#endif

/* Internal Debug */
// #define DEBUG_PARSER

#endif  // CONFIG_H