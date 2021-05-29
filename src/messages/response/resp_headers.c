#include "messages/response/resp_headers.h"

#include "utils/base_convertions.h"

void message_headers_to_bits(message *msg, string *s)
{
    // Id (16 bits)
    string *id = decimal_to_binary(msg->id);
    string_pad_zeroes(&id, 16);
    string_add_str(s, id->arr);
    // QR (1 bit)
    string *qr = decimal_to_binary(msg->qr);
    string_pad_zeroes(&qr, 1);
    string_add_str(s, qr->arr);
    // OPCode (4 bits)
    string *opcode = decimal_to_binary(msg->opcode);
    string_pad_zeroes(&opcode, 4);
    string_add_str(s, opcode->arr);
    // AA (1 bit) -> required in responses (ignore here)
    string *aa = decimal_to_binary(msg->aa);
    string_pad_zeroes(&aa, 1);
    string_add_str(s, aa->arr);
    // TC (1 bit)
    string *tc = decimal_to_binary(msg->tc);
    string_pad_zeroes(&tc, 1);
    string_add_str(s, tc->arr);
    // RD (1 bit)
    string *rd = decimal_to_binary(msg->rd);
    string_pad_zeroes(&rd, 1);
    string_add_str(s, rd->arr);
    // RA (1 bit)
    string *ra = decimal_to_binary(msg->ra);
    string_pad_zeroes(&ra, 1);
    string_add_str(s, ra->arr);
    // Z (3 bits)
    string *z = string_init();
    string_pad_zeroes(&z, 3);
    string_add_str(s, z->arr);
    // RCODE (4 bits) TODO cases other than NO_ERR
    string *rcode = decimal_to_binary(msg->rcode);
    string_pad_zeroes(&rcode, 4);
    string_add_str(s, rcode->arr);
    // QDCOUNT (16 bits)
    string *qdcount = decimal_to_binary(msg->qdcount);
    string_pad_zeroes(&qdcount, 16);
    string_add_str(s, qdcount->arr);
    // ANCOUNT (16 bits)
    string *ancount = decimal_to_binary(msg->ancount);
    string_pad_zeroes(&ancount, 16);
    string_add_str(s, ancount->arr);
    // NSCOUNT (16 bits) TODO if we have to handle Authority section in response
    string *nscount = decimal_to_binary(msg->nscount);
    string_pad_zeroes(&nscount, 16);
    string_add_str(s, nscount->arr);
    // ARCOUNT (16 bits) TODO if we have to handle Additional section in response
    string *arcount = decimal_to_binary(msg->arcount);
    string_pad_zeroes(&arcount, 16);
    string_add_str(s, arcount->arr);

    // Free memory
    string_free(nscount);
    string_free(arcount);
    string_free(qdcount);
    string_free(ancount);
    string_free(z);
    string_free(rcode);
    string_free(rd);
    string_free(ra);
    string_free(aa);
    string_free(tc);
    string_free(opcode);
    string_free(qr);
    string_free(id);
}
