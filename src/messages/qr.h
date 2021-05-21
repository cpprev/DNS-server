#ifndef QR_H
#define QR_H

typedef enum
{
    QR_NONE,
    REQUEST = 0,
    RESPONSE = 1
} QR;

const char *qr_to_string(QR qr);

#endif
