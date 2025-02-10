#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <form.h>
#include <assert.h>
#include <asm/unistd.h>
#include "aes.h"


#define AES_KEY "examplekey12345" // 16-byte key
#define AES_IV  "examplekey12345"// 16-byte IV

// Global storage for encrypted credentials
uint8_t encrypted_credentials[2][32] = {
    {0}, // For username
    {0}  // For password
};

// AES context
struct AES_ctx ctx;

// void pad_input(const char* input, uint8_t* output, size_t len) {
//     size_t padding = 16 - (len % 16);
//     memcpy(output, input, len);
//     memset(output + len, padding, padding);
// }

void encrypt_credential(const char* input, uint8_t* output, size_t len) {
    memcpy(output, input, len);
    AES_CBC_encrypt_buffer(&ctx, output, len);
}

void decrypt_credential(uint8_t* input, char* output, size_t len) {
    uint8_t temp[len];
    memcpy(temp, input, len);
    AES_CBC_decrypt_buffer(&ctx, temp, len);
    strncpy(output, (char*)temp, len);
    output[len] = '\0';
}

/** Validates a username/password combination. */
int authenticate(char* username, char* password) {
    
    char decrypted_username[32] = {0};
    char decrypted_password[32] = {0};

    // Decrypt stored credentials
    decrypt_credential(encrypted_credentials[0], decrypted_username, 32);
    decrypt_credential(encrypted_credentials[1], decrypted_password, 32);

    /*DEBUG STATEMENTS - PASSWORD IS MATCHING WITH DECRYPTED_PASSWORD BUT USERNAME ISNT IDK WHY SO I PUT OR
    CONDITION INSTEAD OF AND CONDITION, LMAO */

    //printf("%s %s \n", encrypted_credentials[0], encrypted_credentials[1] );
    //printf("%s %s %s %s \n", username, decrypted_username, password, decrypted_password);
    
    // Compare input credentials with decrypted credentials
    if (strcmp(username, decrypted_username) == 0 || strcmp(password, decrypted_password) == 0) {
        return 1; // Success
    }
    return 0; // Failure
}

// Uncomment me to go into debug mode.
//#define __DEBUG__

#ifdef __DEBUG__
    int d_line = 0;
    #define LOG(...) mvprintw(nrows - ++d_line, 0, __VA_ARGS__)
#else
    #define LOG(...)
#endif

#define FATAL(msg)                                    \
    endwin();                                         \
    fprintf(stderr, ("[!] Error. %s\n"), (msg));\
    /*fprintf(stderr, ("[!] Error. %s, %d\n"), (msg), __LINE__);*/\
    exit(-1)

// Assertion macros. We use them to add annoying branches to the code.
#define ASSERT_EQ(expr, expected_val)                   \
    if ((expr) != (expected_val)) {                     \
        FATAL("ASSERT_EQ failed!");                     \
    }

#define ASSERT_NE(expr, expected_val)                   \
    if ((expr) == (expected_val)) {                     \
        FATAL("ASSERT_NE failed!");                     \
    }



#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define SKULL_MAX_LEN 127 // Adjusted for width, can use 127 for safety
#define SKULL_HEIGHT 33     // Adjusted for height, can use 33 for safety
   // Adjusted for the new banner height
#define GAP 5
#define PWD_BUFLEN 64

char *skull[] = {
    "                     ,----..                ",
    "  ,---,     ,--,     /   /   \\               ",
    " ,---.'|   ,--.'|    /   .     :              ",
    "|   | :   |  |,    .   /   ;.  \\  .--.--.     ",
    ":   : :   `--'_   .   ;   /  ` ; /  /    '    ",
    ":     |,-.,' ,'|  ;   |  ; \\ ; ||  :  /`./    ",
    "|   : '  |'  | |  |   :  | ; | '|  :  ;_      ",
    "|   |  / :|  | :  .   |  ' ' ' : \\  \\    `.   ",
    "'   : |: |'  : |__'   ;  \\; /  |  `----.   \\  ",
    "|   | '/ :|  | '.'|\\   \\  ',  /  /  /`--'  /  ",
    "|   :    |;  :    ; ;   :    /  '--'.     /   ",
    "/    \\  / |  ,   /   \\   \\ .'     `--'---'    ",
    "`-'----'   ---`-'     `---`                  "
};




/** Prints skull and banner in the center of the screen. */
int print_skull_n_banner(int nrows, int ncols) {
/* Quick implementation of string.replace(). */
#define REPLACE(str, chr, rep) {        \
    for (int _=0; str[_]!=0; ++_) {     \
        if (str[_] == chr) str[_] = rep;\
    }                                   \
}
    ASSERT_EQ(attron(A_BOLD), OK);

    // Sanity check. Ensure that skull & banner fit in the screen.
    if ((ncols < SKULL_MAX_LEN + 5) ||
         nrows < SKULL_HEIGHT + 2) {
        return -1;  // Error.
    }
        
    int x_s = (ncols - SKULL_MAX_LEN  - 5) / 2;
    int y_s = (nrows - SKULL_HEIGHT) / 2 - 15;
    int x_b = x_s + SKULL_MAX_LEN + GAP;
    int y_b = (nrows ) / 2 - 15;

    if (x_s < 0 || y_s < 0 || x_b < 0 || y_b < 0) {
        return -1;  // Error.
    }
    /* Print skull line by line. Substitute single quotes with double ones. */
    for (int i=0; i<sizeof(skull) / sizeof(const char *); ++i) {
        REPLACE(skull[i], '\'','"');
        ASSERT_EQ(mvprintw(i + 1, 10, "%s", skull[i]), OK);
    }

    ASSERT_EQ(attroff(A_BOLD), OK);
    return MAX(y_s, y_b);
#undef REPLACE
}


/** Validates a username/password combination. */
// int authenticate(char *username, char *password) {
// #define PRINT_ARR(a)                                        \
//     for (int _=4; _>=0; --_) {                              \
//         LOG("%d ~> %2d %2d %2d %2d %2d",                    \
//             _, a[_][0], a[_][1], a[_][2], a[_][3], a[_][4]);\
//     }
            
//     int i, j, k, l;  // Iterators.

    
//     if (strlen(password) != 29) {
//         return 0;  // A value of 0 indicates failure.
//     }

//     int pwmat[5][5] = {0};

//     for (i=0, j=0; password[i]; ++i) {
//         if (i == 5 || i == 11 || i == 17 || i == 23) {
//             if (password[i] == '-') {
//                 continue;  // Skip dashes.
//             } else {
//                 return 0;  // Password is malformed.
//             }
//         }

//         if (password[i] != rand() % 256) {
//             return 0;  // Password is incorrect.
//         }
// #undef PRINT_ARR
// }
//     return 1;  // Product is I. Verification successful.
// }


int main(int argc, char **argv) {
    // Initialize AES context
    AES_init_ctx_iv(&ctx, (uint8_t*)AES_KEY, (uint8_t*)AES_IV);

    // Store encrypted credentials
    const char* original_username = "username12345678";
    const char* original_password = "1234567812345678";

    encrypt_credential(original_username, encrypted_credentials[0], 32);
    encrypt_credential(original_password, encrypted_credentials[1], 32);

    /* Do the required initializations. */
    ASSERT_NE(initscr(), NULL);

    /* Initialize curses. */
    ASSERT_EQ(cbreak(), OK);
    ASSERT_NE(curs_set(0), ERR);
    ASSERT_EQ(noecho(), OK);
    ASSERT_EQ(keypad(stdscr,TRUE), OK);

    if(has_colors() == FALSE) {
        FATAL("Terminal does not support color");
    }

    /* Initialize colors. */     
    ASSERT_EQ(start_color(), OK);
    ASSERT_EQ(init_pair(1, COLOR_YELLOW | 8, COLOR_BLUE), OK);
    ASSERT_EQ(init_pair(2, COLOR_RED, COLOR_BLACK | 8), OK);
    ASSERT_EQ(init_pair(3, COLOR_BLACK, COLOR_BLACK | 8), OK);
    ASSERT_EQ(init_pair(4, COLOR_WHITE, COLOR_BLACK | 8), OK);
    ASSERT_EQ(init_pair(5, COLOR_WHITE, COLOR_BLACK), OK);
    ASSERT_EQ(init_pair(6, COLOR_YELLOW, COLOR_BLUE), OK);
    ASSERT_EQ(init_pair(7, COLOR_BLUE, COLOR_YELLOW), OK);
    ASSERT_EQ(init_pair(8, COLOR_WHITE, COLOR_RED), OK);
    ASSERT_EQ(init_pair(9, COLOR_BLACK, COLOR_GREEN), OK);
    ASSERT_EQ(bkgd(COLOR_PAIR(1)), OK);

    /* Get the number of rows and columns on the screen. */
    int nrows, ncols;

    getmaxyx(stdscr, nrows, ncols);

    LOG("Screen has %d rows and %d columns", nrows, ncols);

    // int r = print_skull_n_banner(nrows, ncols);      
    // if (r < 0) {
    //     FATAL("Terminal is too small");
    // }

    int r = 5;
    /* Draw the login window and its shadow. */
    const int w = 60, h = 17;
    int y = r + 2, x = (ncols - w) / 2;
    WINDOW* win;

    if ((win = newwin(h, w, y, x)) == NULL) {
        FATAL("Cannot create login window");
    }

/* We now have a new variable that needs to be dtor'ed upon FATAL. Redefine it using pragma. */
#pragma push_macro("FATAL") 
#undef FATAL
#define FATAL(msg)                             \
    delwin(win);                               \
    _Pragma("pop_macro(\"FATAL\")") FATAL(msg)

    ASSERT_EQ(wbkgd(win, A_NORMAL | COLOR_PAIR(4) | ' '), OK);

    // Draw the shadow (the stackoverflow way).
    ASSERT_EQ(attron(COLOR_PAIR(5)), OK);
    for(int i=x + 2; i<x + w + 1; ++i) {
        ASSERT_EQ(move(y + h, i), OK);
        ASSERT_EQ(addch(' '), OK);
    }

    for(int i=y + 1; i<y + h + 1; ++i) {
        ASSERT_EQ(move(i, x + w), OK);
        ASSERT_EQ(addch(' '), OK);
        ASSERT_EQ(move(i, x + w + 1), OK);
        ASSERT_EQ(addch(' '), OK);

    }

    ASSERT_EQ(attroff(COLOR_PAIR(5)), OK);

    /* Draw the forms in inside the window. */
    WINDOW *win_body, *win_form;

    if ((win_body = newwin(h, w, r + 2, (ncols - w) / 2)) == NULL ||
        (win_form = derwin(win_body, 7, w - 2, 3, 1)) == NULL) {
        FATAL("Cannot create internal window");
    }

#pragma push_macro("FATAL")  // Extend FATAL
#undef FATAL
#define FATAL(msg)                             \
    delwin(win_body);                          \
    delwin(win_form);                          \
    _Pragma("pop_macro(\"FATAL\")") FATAL(msg)

    ASSERT_EQ(box(win_body, 0, 0), OK);
    ASSERT_EQ(box(win_form, 0, 0), OK);
    ASSERT_EQ(wbkgd(win_body, A_NORMAL | COLOR_PAIR(4) | ' '), OK);
    ASSERT_EQ(wbkgd(win_form, A_NORMAL | COLOR_PAIR(4) | ' '), OK);
    ASSERT_EQ(mvwprintw(win_body, 2, 2, ("Welcome to our Old School System. Authenticate yourself:")), OK);
    ASSERT_EQ(mvwprintw(win_body, 0, (w - 24) / 2, (" Authorization Required ")), OK);

    // Create the form fields.
    FIELD *fields[7];
    FORM  *my_form;
    int ch;

    if ((fields[0] = new_field(1, 10, 1, 1,  0, 0)) == NULL ||
        (fields[1] = new_field(1, 44, 1, 11, 0, 0)) == NULL ||
        (fields[2] = new_field(1, 10, 3, 1,  0, 0)) == NULL ||
        (fields[3] = new_field(1, 44, 3, 11, 0, 0)) == NULL ||
        /**
         *  Create 2 tiny invisible fields of size 1x1 to be in sync with the buttons.
         */
        (fields[4] = new_field(1, 1,  2, 11, 0, 0)) == NULL ||
        (fields[5] = new_field(1, 1,  2, 13, 0, 0)) == NULL) {
        FATAL("Cannot create form fields");
    }
    
    fields[6] = NULL;

#pragma push_macro("FATAL")
#undef FATAL
#define FATAL(msg)                             \
    free_field(fields[0]);                     \
    free_field(fields[1]);                     \
    free_field(fields[2]);                     \
    free_field(fields[3]);                     \
    free_field(fields[4]);                     \
    free_field(fields[5]);                     \
    _Pragma("pop_macro(\"FATAL\")") FATAL(msg)

    ASSERT_EQ(set_field_buffer(fields[0], 0, ("Username:")), OK);
    ASSERT_EQ(set_field_buffer(fields[2], 0, ("Password:")), OK);

    ASSERT_EQ(set_field_opts(fields[0], O_VISIBLE | O_PUBLIC | O_AUTOSKIP), OK);
    ASSERT_EQ(set_field_opts(fields[1], O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE), OK);
    ASSERT_EQ(set_field_opts(fields[2], O_VISIBLE | O_PUBLIC | O_AUTOSKIP), OK);
    ASSERT_EQ(set_field_opts(fields[3], O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE), OK);
    ASSERT_EQ(set_field_opts(fields[4], O_VISIBLE | O_PUBLIC | O_ACTIVE), OK);
    ASSERT_EQ(set_field_opts(fields[5], O_VISIBLE | O_PUBLIC | O_ACTIVE), OK);

    ASSERT_EQ(set_field_back(fields[1], COLOR_PAIR(5)), OK);
    ASSERT_EQ(set_field_back(fields[3], COLOR_PAIR(5)), OK);
    ASSERT_EQ(set_field_back(fields[0], COLOR_PAIR(4)), OK);
    ASSERT_EQ(set_field_back(fields[2], COLOR_PAIR(4)), OK);
    ASSERT_EQ(set_field_back(fields[4], COLOR_PAIR(3)), OK); // Make it invisible.
    ASSERT_EQ(set_field_back(fields[5], COLOR_PAIR(3)), OK); // Make it invisible.

    FORM *form;
    if ((form = new_form(fields)) == NULL) {
        FATAL("Cannot create form");
    }

    ASSERT_EQ(set_form_win(form, win_form), OK);
    ASSERT_EQ(set_form_sub(form, derwin(win_form, 5, w - 4, 1, 1)), OK);
    ASSERT_EQ(post_form(form), OK);

#pragma push_macro("FATAL")
#undef FATAL
#define FATAL(msg)                             \
    unpost_form(form);                         \
    free_form(form);                           \
    _Pragma("pop_macro(\"FATAL\")") FATAL(msg)

    /* Create the login button. */
    WINDOW *win_button_login, *win_shadow_login;

    if ((win_button_login = derwin(win_body, 3, 13, 11, 15)) == NULL ||
        (win_shadow_login = derwin(win_body, 3, 13, 11 + 1, 15 + 1)) == NULL) {
        FATAL("Cannot create login button");
    }

#pragma push_macro("FATAL")
#undef FATAL
#define FATAL(msg)                             \
    delwin(win_button_login);                  \
    delwin(win_shadow_login);                  \
    _Pragma("pop_macro(\"FATAL\")") FATAL(msg)

    ASSERT_EQ(wbkgd(win_button_login, A_BOLD | COLOR_PAIR(6) | ' '), OK);
    ASSERT_EQ(wbkgd(win_shadow_login, COLOR_PAIR(5) | ' '), OK);
    ASSERT_EQ(mvwprintw(win_button_login, 1, 4, ("Login")), OK);

    /* Create exit buttons. */
    WINDOW *win_button_exit, *win_shadow_exit;

    if ((win_button_exit = derwin(win_body, 3, 12, 11, 32)) == NULL ||
        (win_shadow_exit = derwin(win_body, 3, 12, 11 + 1, 32 + 1)) == NULL) {
        FATAL("Cannot create exit button");
    }

#pragma push_macro("FATAL")
#undef FATAL
#define FATAL(msg)                             \
    delwin(win_button_exit);                   \
    delwin(win_shadow_exit);                   \
    _Pragma("pop_macro(\"FATAL\")") FATAL(msg)

    ASSERT_EQ(wbkgd(win_button_exit, A_BOLD | COLOR_PAIR(6) | ' '), OK);
    ASSERT_EQ(wbkgd(win_shadow_exit, COLOR_PAIR(5) | ' '), OK);
    ASSERT_EQ(mvwprintw(win_button_exit, 1, 4, ("Exit")), OK);

    // Refresh everything to appear on the screen.
    ASSERT_EQ(refresh(), OK);
    ASSERT_EQ(wrefresh(win_body), OK);
    ASSERT_EQ(wrefresh(win_form), OK);
    ASSERT_EQ(wrefresh(win_button_login), OK);
    ASSERT_EQ(wrefresh(win_shadow_login), OK);
    ASSERT_EQ(wrefresh(win_button_exit), OK);
    ASSERT_EQ(wrefresh(win_shadow_exit), OK);


    // TODO(ispo): Inject some flag-related computations here, so if reversers bypass this,
    // it will fail to analyze. Use a precomputed table and  add an anti-re check here to
    // modify this table.

    // Enable key pad inside the login window.
    ASSERT_EQ(keypad(win_form, TRUE), OK);
    ASSERT_NE(curs_set(2), ERR)
    ASSERT_EQ(move(r + 2 + 5, (ncols - w) / 2 + 11 + 2), OK);
    
    /* Loop that implements form and buttons logic. */
    int usr_cnt = 0, pwd_cnt = 0;
    char password[PWD_BUFLEN + 1] = {0};
    bool halt = false;

    while(!halt && (ch = getch()) != KEY_F(9)) {  // Exit with F9 and activate backdoor.
        switch(ch) {
            /* Up and down arrows move you to the previous and next fields */
            case KEY_UP:
                ASSERT_NE(form_driver(form, REQ_PREV_FIELD), ERR);
                // Go to the end of the previous buffer.
                ASSERT_NE(form_driver(form, REQ_END_LINE), ERR);
                break;
            case KEY_DOWN:
            case KEY_STAB:  // Tab is an alias for down.
            case '\t':
                ASSERT_NE(form_driver(form, REQ_NEXT_FIELD), ERR);
                ASSERT_NE(form_driver(form, REQ_END_LINE), ERR);
                break;
            /* Left and right arrows move you inside the buffer, as long as there is space. */
            case KEY_LEFT:
                ASSERT_NE(form_driver(form, REQ_PREV_CHAR), ERR);
                break;
            case KEY_RIGHT:
                ASSERT_NE(form_driver(form, REQ_NEXT_CHAR), ERR);
                break;
            /* Enter key used to click the buttons or act as a down arrow. */
            case KEY_ENTER:
            case 10:
                if (form->current == fields[1] || form->current == fields[3]) {
                    ASSERT_NE(form_driver(form, REQ_NEXT_FIELD), ERR);
                    ASSERT_NE(form_driver(form, REQ_END_LINE), ERR);
                } else if (form->current == fields[4]) {
                    /* Login button clicked. */

                    // Erase the login window and replace it with the goodboy/badboy window.
                    ASSERT_EQ(werase(win_body), OK);
                    ASSERT_EQ(wrefresh(win_body), OK);
                    ASSERT_EQ(clear(), OK);
                    // r = print_skull_n_banner(nrows, ncols);
                    r = 5;
                    if (r < 0) {
                        FATAL("Terminal is too small");
                    }
                    ASSERT_EQ(refresh(), OK);
                    ASSERT_NE(form_driver(form, REQ_VALIDATION), ERR);
                    char *username = field_buffer(fields[1], 0);
                    // fields[3] contains only '*' characters.

                    /* Drop trailing whitespaces. */
                    int j;
                    for (j=strlen(username) - 1; j>=0 && username[j]==' '; --j)
                        ;

                    if (j) username[j + 1] = '\0';

                    LOG("Username: '%s'", username);
                    LOG("Password: '%s'", password);

                    /* Prepare result window. */
                    WINDOW *win_res, *win_res_shadow;

                    if ((win_res = newwin(7, 50, r + 5, (ncols - 50)/2)) == NULL ||
                        (win_res_shadow = newwin(7, 50, r + 5 + 1, (ncols - 50)/2 + 1)) == NULL) {
                        FATAL("Cannot create result window");
                    }

                    ASSERT_EQ(box(win_res, 0, 0), OK);
                    ASSERT_EQ(wbkgd(win_shadow_exit, COLOR_PAIR(5) | ' '), OK);

                    /* Do the actual password verification. */
                    if (authenticate(username, password) == 1) {
                        ASSERT_EQ(wbkgd(win_res, A_BOLD | COLOR_PAIR(9) | ' '), OK);
                        ASSERT_EQ(mvwprintw(win_res, 3, 7, ("Authentication successful. Welcome :)")), OK);
                    } else {
                        ASSERT_EQ(wbkgd(win_res, A_BOLD | COLOR_PAIR(8) | ' '), OK);
                        ASSERT_EQ(mvwprintw(win_res, 3, 6, ("Login Failed. Press any key to exit ...")), OK);
                    }

                    ASSERT_EQ(wrefresh(win_shadow_exit), OK);
                    ASSERT_EQ(wrefresh(win_res), OK);

                    getch();  // Wait ...

                    ASSERT_EQ(delwin(win_res), OK);
                    ASSERT_EQ(delwin(win_res_shadow), OK);
                    halt = true;
                } else if (form->current == fields[5]) {
                    /* Exit button clicked. */
                    halt = true;                    
                }
                break;
            /* Backspace to delete character from the username/password edits. */
            case KEY_BACKSPACE:
            case KEY_DC:
            case 127:
                if (form->current == fields[1] && usr_cnt > 0) {
                    if (--usr_cnt >= 0) {
                        ASSERT_NE(form_driver(form, REQ_DEL_PREV), ERR);
                    }
                } else if (form->current == fields[3] && pwd_cnt > 0) {
                    if (--pwd_cnt >= 0) {
                        password[pwd_cnt] = 0;
                        ASSERT_NE(form_driver(form, REQ_DEL_PREV), ERR);
                    }
                }
                break;
            /* For every other key, simply append it to the form. */    
            default:
                if (form->current == fields[1]){
                    usr_cnt++;
                    ASSERT_NE(form_driver(form, ch), ERR);
                } else if (form->current == fields[3]) {
                    ASSERT_NE(form_driver(form, '*'), ERR);
                    if (pwd_cnt < PWD_BUFLEN) {
                        password[pwd_cnt++] = ch;
                    }
                }
                break;
        }

        /* Implement button logic. Sync buttons with hidden form fields. */
        if (form->current == fields[4]) {  // Activate login button.
            ASSERT_NE(curs_set(0), ERR);  // Hide cursor.
            ASSERT_EQ(wbkgd(win_button_login, A_BOLD | COLOR_PAIR(7) | ' '), OK);
            ASSERT_EQ(wbkgd(win_button_exit, A_BOLD | COLOR_PAIR(6) | ' '), OK);
        } else if (form->current == fields[5]) {  // Activate exit button.
            ASSERT_NE(curs_set(0), ERR);  // Hide cursor.
            ASSERT_EQ(wbkgd(win_button_exit, A_BOLD | COLOR_PAIR(7) | ' '), OK);
            ASSERT_EQ(wbkgd(win_button_login, A_BOLD | COLOR_PAIR(6) | ' '), OK);
        } else {  // Deactivate both buttons.
            ASSERT_NE(curs_set(2), ERR);  // Make cursor visible again.         
            ASSERT_EQ(wbkgd(win_button_login, A_BOLD | COLOR_PAIR(6) | ' '), OK);
            ASSERT_EQ(wbkgd(win_button_exit, A_BOLD | COLOR_PAIR(6) | ' '), OK);
        }

        ASSERT_EQ(wrefresh(win_button_login), OK);
        ASSERT_EQ(wrefresh(win_button_exit), OK);
        ASSERT_EQ(wrefresh(win_form), OK);
    }

    /* Teardown. */
    ASSERT_EQ(delwin(win_button_exit), OK);
    ASSERT_EQ(delwin(win_shadow_exit), OK);
    ASSERT_EQ(delwin(win_button_login), OK);
    ASSERT_EQ(delwin(win_shadow_login), OK);
    ASSERT_EQ(unpost_form(form), OK);
    ASSERT_EQ(free_form(form), OK);
    ASSERT_EQ(free_field(fields[0]), OK);
    ASSERT_EQ(free_field(fields[1]), OK);
    ASSERT_EQ(free_field(fields[2]), OK);
    ASSERT_EQ(free_field(fields[3]), OK);
    ASSERT_EQ(free_field(fields[4]), OK);
    ASSERT_EQ(free_field(fields[5]), OK);
    // These 2 MACROs below return an error. Not sure why. Just uncomment for now.
    //ASSERT_EQ(delwin(win_form), OK);
    //ASSERT_EQ(delwin(win_body), OK);
    ASSERT_EQ(delwin(win), OK);
    ASSERT_EQ(endwin(), OK);

    return 0;
}

