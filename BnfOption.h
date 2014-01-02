#ifndef __BNF_OPTION__
#define __BNF_OPTION__

namespace Bnf
{

struct BnfOption
{
    enum e_blanks
    {
        SKIP_BLANK,
        NOT_SKIP_BLANK,
        CHECK_BLANCKS
    };
    
    BnfOption(e_blanks opt) : blanks(opt), reinit(true), loop(-1) {};
    BnfOption() : reinit(true), loop(-1) {};

    e_blanks         blanks;
    bool             reinit;
    int              loop;
};

}

#endif