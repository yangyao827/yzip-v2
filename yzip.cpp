#include "system.h"
#include "getopt.h"
#include "version/version_etc.h"
#include "error/error.h"
#include "version/app_info.h"
#include "yzip_struct.h"
#include "file_handle/file_handle.h"
#include "string.h"
using namespace std;
const char *program_name = "yzip";
static struct option const longopts[] =
        {
                {"decompress", no_argument,       NULL, 'd'},
                {"encrypt",    required_argument, NULL, 'e'},
                {"list",       required_argument, NULL, 'l'},
                {"output",     required_argument, NULL, 'o'},
                {"recursive",  no_argument,       NULL, 'r'},
                {"test",       required_argument, NULL, 't'},
                {"verbose",    no_argument,       NULL, 'v'},
                {GETOPT_HELP_OPTION_DECL},
                {GETOPT_VERSION_OPTION_DECL},
                {NULL, 0,                         NULL, 0}
        };

    void usage(int status) {
    if (status != EXIT_SUCCESS)
        emit_try_help ();
    else {
        fputs(_("\
Usage: yzip [OPTION]... [FILE]...\n\
Compress or uncompress FILEs (by default, compress FILES in-place).\n\
\n\
Mandatory arguments to long options are mandatory for short options too.\n\
\n\
  -d, --decompress         decompress\n\
  -e, --encrypt            encrypt\n\
  -l, --list               list the contents of an archive\n\
  -o, --output             output file path\n\
  -r, --recursive          recurse into directories\n\
  -t, --test               test the package\n\
  -v, --verbose            verbose mode\n\
      --help           display this help and exit\n\
      --version        output version information and exit\n\
"), stdout);

    }
    exit(status);
}

int main(int argc, char *argv[]) {
    int optc;
    struct YzipOptions options;//参数对象，全局
    while ((optc = getopt_long(argc, argv, "de:l:o:rt:v", longopts, nullptr)) != -1) {
        switch (optc) {
            case 'd':
                options.flag = 1;
                break;
            case 'e':
                if (strlen(optarg) < 6)
                    error_msg(EXIT_FAILURE, PROGRAM_NAME,
                              _("Passwords are too short, please make sure your password is 6-16 characters long"),
                              nullptr);
                else if (strlen(optarg) > 16)
                    error_msg(EXIT_FAILURE, PROGRAM_NAME,
                              _("Passwords are too long, please make sure your password is 6-16 characters long"),
                              nullptr);
                else
                    options.password = optarg;
                break;
            case 'l':
                list_pkg_file(optarg);
                break;
            case 'o':
                options.out_path = optarg;
                break;
            case 'r':
                options.is_res = true;
                break;
            case 't':
                check_file(optarg, true);
                break;
            case 'v':
                options.show_info = true;
                break;
            case_GETOPT_HELP_CHAR;
            case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
            default:
                usage(EXIT_FAILURE);
        }
    }
    if (optind == argc) {
        error_msg(EXIT_SUCCESS, program_name, _("missing operand"), nullptr);
        usage(EXIT_FAILURE);
    }
    while (optind < argc) {
        options.operand.emplace_back(argv[optind]);
        optind++;
    }
    file_handle(options);
    return 0;

}
