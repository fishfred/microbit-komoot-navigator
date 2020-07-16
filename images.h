/*
    Arrow icons. Stored in flash.
*/

const uint8_t arrow_go_straight[] __attribute__ ((aligned (4))) = { 0xff, 0xff, 5, 0, 4, 0,
    0, 0, 1, 0, 0,
    0, 1, 1, 1, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0
};
const uint8_t arrow_slight_left[] __attribute__ ((aligned (4))) = { 0xff, 0xff, 5, 0, 4, 0,
    1, 1, 1, 0, 0,
    1, 1, 0, 0, 0,
    1, 0, 1, 0, 0,
    0, 0, 0, 1, 0
};
const uint8_t arrow_left[] __attribute__ ((aligned (4))) = { 0xff, 0xff, 5, 0, 4, 0,
    0, 1, 0, 0, 0,
    1, 1, 1, 1, 0,
    0, 1, 0, 1, 0,
    0, 0, 0, 1, 0
};
const uint8_t arrow_sharp_left[] __attribute__ ((aligned (4))) = { 0xff, 0xff, 5, 0, 4, 0,
    0, 0, 0, 1, 0,
    1, 0, 1, 0, 0,
    1, 1, 0, 0, 0,
    1, 1, 1, 0, 0
};
const uint8_t arrow_sharp_right[] __attribute__ ((aligned (4))) = { 0xff, 0xff, 5, 0, 4, 0,
    0, 1, 0, 0, 0,
    0, 0, 1, 0, 1,
    0, 0, 0, 1, 1,
    0, 0, 1, 1, 1
};
const uint8_t arrow_right[] __attribute__ ((aligned (4))) = { 0xff, 0xff, 5, 0, 4, 0,
    0, 0, 0, 1, 0,
    0, 1, 1, 1, 1,
    0, 1, 0, 1, 0,
    0, 1, 0, 0, 0
};
const uint8_t arrow_slight_right[] __attribute__ ((aligned (4))) = { 0xff, 0xff, 5, 0, 4, 0,
    0, 0, 1, 1, 1,
    0, 0, 0, 1, 1,
    0, 0, 1, 0, 1,
    0, 1, 0, 0, 0
};
const uint8_t arrow_fork_right[] __attribute__ ((aligned (4))) = { 0xff, 0xff, 5, 0, 4, 0,
    1, 0, 1, 1, 1,
    1, 0, 0, 1, 1,
    1, 0, 1, 0, 1,
    1, 1, 0, 0, 0
};
const uint8_t arrow_fork_left[] __attribute__ ((aligned (4))) = { 0xff, 0xff, 5, 0, 4, 0,
    1, 1, 1, 0, 1,
    1, 1, 0, 0, 1,
    1, 0, 1, 0, 1,
    0, 0, 0, 1, 1
};
const uint8_t arrow_u_turn[] __attribute__ ((aligned (4))) = { 0xff, 0xff, 5, 0, 4, 0,
    0, 1, 1, 1, 1,
    0, 1, 0, 0, 1,
    1, 1, 1, 0, 1,
    0, 1, 0, 0, 1
};
const uint8_t arrow_1[] __attribute__ ((aligned (4))) = { 0xff, 0xff, 5, 0, 4, 0,
    0, 1, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 1, 1, 1, 0
};
const uint8_t arrow_2[] __attribute__ ((aligned (4))) = { 0xff, 0xff, 5, 0, 4, 0,
    0, 1, 0, 1, 0,
    0, 1, 0, 1, 0,
    0, 1, 0, 1, 0,
    0, 1, 0, 1, 0
};
const uint8_t arrow_3[] __attribute__ ((aligned (4))) = { 0xff, 0xff, 5, 0, 4, 0,
    1, 0, 1, 0, 1,
    1, 0, 1, 0, 1,
    1, 0, 1, 0, 1,
    1, 0, 1, 0, 1
};
const uint8_t line[] __attribute__ ((aligned (4))) = { 0xff, 0xff, 5, 0, 1, 0,
    1, 1, 1, 1, 1
};

const uint8_t loading[] __attribute__ ((aligned (4))) = { 0xff, 0xff, 10, 0, 1, 0,
    1, 0, 0, 0, 0,      0, 1, 0, 0, 0
};

const uint8_t disconnected[] __attribute__ ((aligned (4))) = { 0xff, 0xff, 5, 0, 5, 0,
    1, 0, 0, 0, 1,
    0, 1, 0, 1, 0,
    0, 0, 1, 0, 0,
    0, 1, 0, 1, 0,
    1, 0, 0, 0, 1
};

const uint8_t connected[] __attribute__ ((aligned (4))) = { 0xff, 0xff, 5, 0, 5, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 1,
    0, 0, 0, 1, 0,
    1, 0, 1, 0, 0,
    0, 1, 0, 0, 0
};