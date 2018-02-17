#include "pdu.h"
#include "oldcommon/magic/magic.h"

using namespace MAGIC;

#define GSM_SMS_FORMAT_MASK			  0x30
#define GSM_SMS_FORMAT_7BIT			  0x00
#define GSM_SMS_FORMAT_16BIT			0x10
#define GSM_SMS_FORMAT_HEX				0x20
#define GSM_SMS_FORMAT_BINARY		  0x30
#define GSM_SMS_LONG_VALID				0x02
/* ----------------------------------------------- *
*  PDU message header position
* ----------------------------------------------- */
#define PDU_TYPE_OFFSET                     1
#define PDU_OA_LEN_OFFSET                   2
#define PDU_OA_TYPE_OFFSET                  3
#define PDU_OA_DATA_OFFSET                  4

#define PDU_DCS_OFFSET                      (2 + 1 + 1 + 1) // offset mainfrom oa + len SCA + len OA + type OA
#define PDU_UDL_OFFSET                      (2 + 1 + 1 + 1 + 7 + 1) // add SCTS offset
#define PDU_DATA_OFFSET                     (PDU_UDL_OFFSET + 1)

#define PDU_TYPE_VALUE          (pdu_sms_binary[PDU_TYPE_OFFSET + (pdu_sms_binary[0])])
#define PDU_OA_LEN_VALUE        (pdu_sms_binary[PDU_OA_LEN_OFFSET + (pdu_sms_binary[0])])
#define PDU_OA_TYPE_VALUE       (pdu_sms_binary[PDU_OA_TYPE_OFFSET + (pdu_sms_binary[0])])
#define PDU_OA_DATA_VALUE(x)    (pdu_sms_binary[(x) + PDU_OA_DATA_OFFSET + (pdu_sms_binary[0])])
#define PDU_OA_LEN_EVEN_VALUE   (PDU_OA_LEN_VALUE + (PDU_OA_LEN_VALUE % 2))
#define PDU_DCS_VALUE           (pdu_sms_binary[(pdu_sms_binary[0]) + (PDU_OA_LEN_EVEN_VALUE >> 1) + PDU_DCS_OFFSET])
#define PDU_UDL_VALUE				    (pdu_sms_binary[(pdu_sms_binary[0]) + (PDU_OA_LEN_EVEN_VALUE >> 1) + PDU_UDL_OFFSET])
#define PDU_DATA_POINTER			  (&pdu_sms_binary[(pdu_sms_binary[0]) + (PDU_OA_LEN_EVEN_VALUE >> 1) + PDU_DATA_OFFSET])

const int32_t MAX_PHONE_LEN = 15;
uint8_t pdu_sms_binary[256];
char phone_num[MAX_PHONE_LEN + 1];
//static uint8_t myin_phone[] = {"+79299095608"};


uint8_t pdu_RX_Options;

uint8_t* GSM_Save_Byte_As_Hex(uint8_t* DstPtr, uint8_t Byte)
{
  if (Byte >= 0xA0) *DstPtr++ = (((Byte - 0xA0) & 0xF0) >> 4) + 'A';
  else *DstPtr++ = ((Byte & 0xF0) >> 4) + '0';

  Byte &= 0x0F;

  if (Byte >= 0x0A) *DstPtr++ = (Byte - 0x0A) + 'A';
  else *DstPtr++ = Byte + '0';

  return (DstPtr);
}

const char* GetPtrToPhone()
{
//  return myin_phone;
  return phone_num;
}

/* ------------------------------------------------------------------------- */
int32_t FromPduToAscii(char* ascii_text, const char* pdu_text, int32_t lenght)
{
  uint8_t c;
  uint8_t Cntr, CRChi, CRClo;
  uint8_t* PDUPtr;
  const char* p;
  uint8_t HiLoHex;
  int32_t retval = 0;
  char* pphone_num = phone_num;

  if (lenght == 0)
    return 0;

  /* request availible SMS */
  p = pdu_text;
  /* Take hex data from ASCII hex format */
  int i = 0;

  while (i < 256)
  {
    c = p[i];

    if (c >= '0' && c <= '9')
      c -= '0';
    else if (c >= 'A' && c <= 'F')
      c = c - 'A' + 0x0A;
    else if (c == '\0')
      break;

    if (!(i % 2))
      pdu_sms_binary[i >> 1] = c << 4;
    else
      pdu_sms_binary[i >> 1] |= c;

    i++;
  }

//	std::sprintf(cTempBuf, AT_DELETE_ALL_MESSAGE, msgid);
  /* Check valid parse functions */
  int32_t tmp1 = i >> 1;
  int32_t tmp2 = 1 + (pdu_sms_binary[0] + lenght);
  
  if (tmp1 != tmp2)
    /* if common len (i/2) non equal summ scanning values -> error */
    return 0;

  /* check SMS-DELIVER status byte xxxxxx00b must be */
  if ((PDU_TYPE_VALUE & 0x03) != 0x00)
    return 0;

  /* Get type of adress value */
  c = PDU_OA_TYPE_VALUE; // type-of-address
  c &= 0x70;

  /* if international bit set '+' add to phone */
  if (c == 0x10)
    *pphone_num++ = '+';

  if (c < 0x50 || PDU_OA_LEN_VALUE < MAX_PHONE_LEN)
  {
    /* all OK it can store phone number \PDU_OA_LEN_EVEN_VALUE
     * macros make PDU_OA_LEN_VALUE even value */
    for (i = 0; i < PDU_OA_LEN_EVEN_VALUE; i += 2)
    {
      c = (PDU_OA_DATA_VALUE(i >> 1));

      if ((c & 0x0F) < 10)
        *pphone_num++ = (c & 0x0f) + '0';

      if (((c >> 4) & 0x0F) < 10)
        *pphone_num++ = (c >> 4 & 0x0F) + '0';
    }

    *pphone_num++ = 0x00;
  }

  c = PDU_DCS_VALUE;
  i = PDU_UDL_VALUE;
  PDUPtr = PDU_DATA_POINTER;

  switch (c & 0xEC)
  {
    case 0x00:

      /* 7-bit text */
      if (i < 2) break;

      lenght = i;
      Cntr = 0;
      HiLoHex = 1;

      for (i = 0; i != lenght; i++)
      {
        c = (PDUPtr[i - (i >> 3) - 1] >> (8 - (i & 0x07))) | (PDUPtr[i - (i >> 3)] <<
            (i & 0x07));
        c &= 0x7F;

        if (c >= '0' && c <= '9') c -= '0';
        else if (c >= 'A' && c <= 'F') c = c - 'A' + 0x0A;
        else if (c >= 'a' && c <= 'f') c = c - 'a' + 0x0A;
        else if ((c == ' ' || c == '.' || c == ',' || c == ';' || c == ':') &&
                 HiLoHex == 1)
          continue;
        else goto _Verbal_7bit;

        if (HiLoHex == 1)
        {
          ascii_text[Cntr] = c << 4;
          HiLoHex = 0;
        }
        else
        {
          ascii_text[Cntr] |= c;
          Cntr++;
          HiLoHex = 1;
        }
      }

      if (Cntr < 4 || HiLoHex == 0)
        break;

      pdu_RX_Options = GSM_SMS_FORMAT_HEX;
      retval = Cntr;
      break;
_Verbal_7bit:

      for (i = 0; i != lenght; i++)
      {
        c = (PDUPtr[i - (i >> 3) - 1] >> (8 - (i & 0x07))) | (PDUPtr[i - (i >> 3)] <<
            (i & 0x07));
        c &= 0x7F;

        if (c >= 'a' && c <= 'z') c &= 0xDF;

        ascii_text[i] = c;
      }

      ascii_text[lenght] = 0x00;
      pdu_RX_Options = GSM_SMS_FORMAT_7BIT;
      retval = lenght;
      break;

    case 0x04:

      // 8-bit data
      if (i < 6)
        break;

      lenght = i;
      cppCrc16::crc16InitValue(CRChi, CRClo);

      for (i = 0; i != lenght; i++)
      {
        c = *PDUPtr++;
        ascii_text[i] = c;
        cppCrc16::crc16UpdateValue(CRChi, CRClo, c);
      }

      if (CRClo != 0 || CRChi != 0)
        break;

      pdu_RX_Options = GSM_SMS_FORMAT_BINARY;
      retval = lenght - 2;
      break;

    case 0x08:

      // 16-bit verbal text
      if (i < 4)
        break;

      lenght = i >> 1;

      for (i = 0; i != lenght; i++)
      {
        c = PDUPtr[1];

        if ((PDUPtr[0]) != 0x00)
        {
          // russian upcase
          if (c >= 0x10 && c <= 0x4F) c = (c + 0xB0) & 0xDF;
        }
        else
        {
          // english upcase
          if (c >= 'a' && c <= 'z') c &= 0xDF;
        }

        ascii_text[i] = c;
        PDUPtr += 2;
      }

      ascii_text[lenght] = 0x00;
      pdu_RX_Options = GSM_SMS_FORMAT_16BIT;
      retval = lenght;
      break;

    default:
      break;
  }

  return retval;
}


int32_t FromAsciiToPdu(char* pdu_text, const char* ascii_text, int32_t lenght, uint8_t options)
{
  uint8_t i, c, ShiftCntr, CRChi, CRClo;
  uint8_t* PDUPtr;
  uint8_t TmpChars[2];
  /* prepare PDU */
  const char* pphone_num = GetPtrToPhone();
  pdu_sms_binary[0] = 0x00;
  /* first octet SMS-SUBMIT */
  pdu_sms_binary[1] = 0x11;
  pdu_sms_binary[2] = 0x00;

  /* telephone number */
  if (pphone_num[0] == 0x00 || lenght < 1)
    return (0);
  else if (pphone_num[0] == '+')
    pdu_sms_binary[4] = 0x91;
  else
    pdu_sms_binary[4] = 0x81;

  i = 0;

  while (i < 32)
  {
    c = *pphone_num++;

    if (c == 0x00)
      break;

    if (c < '0' || c > '9')
      continue;

    if ((i & 1) == 0)
      pdu_sms_binary[5 + (i >> 1)] = (c - '0') | 0xF0;
    else
      pdu_sms_binary[5 + (i >> 1)] &= ((c - '0') << 4) | 0x0F;

    i++;
  }

  pdu_sms_binary[3] = i;
  PDUPtr = &(pdu_sms_binary[5 + ((i + 1) >> 1)]);
  /* TP-PID */
  *PDUPtr++ = 0x00;

  if (options & GSM_SMS_LONG_VALID)
    c = 196; // 30 days
  else
    c = 11; // 1 hour

  switch (options & GSM_SMS_FORMAT_MASK)
  {
    case GSM_SMS_FORMAT_7BIT:
      // 7-bit verbal text
      // TP-DCS
      *PDUPtr++ = 0x00;
      // TP-VP
      *PDUPtr++ = c;

      // TP-User-Data-Length
      if (lenght > 160)
        lenght = 160;

      *PDUPtr++ = lenght;
      c = 0x00;
      ShiftCntr = 0;

      for (i = 0; i != lenght; i++)
      {
        TmpChars[0] = (ascii_text[i]) & 0x7F;
        c |= (TmpChars[0]) << ShiftCntr;
        ShiftCntr += 7;

        if (ShiftCntr >= 8)
        {
          *PDUPtr++ = c;
          ShiftCntr -= 8;
          c = (TmpChars[0]) >> (7 - ShiftCntr);
        }
      }

      if (ShiftCntr != 0) *PDUPtr++ = c;

      break;

    case GSM_SMS_FORMAT_16BIT:
      // 16-bit verbal text
      // TP-DCS
      *PDUPtr++ = 0x08;
      // TP-VP
      *PDUPtr++ = c;

      // TP-User-Data-Length
      if (lenght > 70) lenght = 70;

      *PDUPtr++ = lenght << 1;

      for (i = 0; i != lenght; i++)
      {
        c = ascii_text[i];

        if (c >= 0xC0)
        {
          PDUPtr[0] = 0x04;
          PDUPtr[1] = c - 0xB0;
        }
        else
        {
          PDUPtr[0] = 0x00;
          PDUPtr[1] = c;
        }

        PDUPtr += 2;
      }

      break;

    case GSM_SMS_FORMAT_HEX:
      // 7-bit hex text
      // TP-DCS
      *PDUPtr++ = 0x00;
      // TP-VP
      *PDUPtr++ = c;

      // TP-User-Data-Length
      if (lenght > 80) lenght = 80;

      *PDUPtr++ = lenght << 1;
      c = 0x00;
      ShiftCntr = 0;

      for (i = 0; i != lenght; i++)
      {
        GSM_Save_Byte_As_Hex(TmpChars, ascii_text[i]);
        // hi hex char
        c |= (TmpChars[0]) << ShiftCntr;
        ShiftCntr += 7;

        if (ShiftCntr >= 8)
        {
          *PDUPtr++ = c;
          ShiftCntr -= 8;
          c = (TmpChars[0]) >> (7 - ShiftCntr);
        }

        // lo hex char
        c |= (TmpChars[1]) << ShiftCntr;
        ShiftCntr += 7;

        if (ShiftCntr >= 8)
        {
          *PDUPtr++ = c;
          ShiftCntr -= 8;
          c = (TmpChars[1]) >> (7 - ShiftCntr);
        }
      }

      if (ShiftCntr != 0) *PDUPtr++ = c;

      break;

    case GSM_SMS_FORMAT_BINARY:
      // 8-bit data
      // TP-DCS
      *PDUPtr++ = 0x04;
      // TP-VP
      *PDUPtr++ = c;

      // TP-User-Data-Length
      if (lenght > 138) lenght = 138;

      *PDUPtr++ = lenght + 2;
      cppCrc16::crc16InitValue(CRChi, CRClo);

      for (i = 0; i != lenght; i++)
      {
        c = ascii_text[i];
        *PDUPtr++ = c;
        cppCrc16::crc16UpdateValue(CRChi, CRClo, c);
      }

      *PDUPtr++ = CRClo;
      *PDUPtr++ = CRChi;
      break;
  }

  // send
  i = (uint8_t)(PDUPtr - pdu_sms_binary);
  uint8_t retval = i - 1;
  PDUPtr = pdu_sms_binary;

  for (; i != 0; i--)
  {
    GSM_Save_Byte_As_Hex(TmpChars, *PDUPtr++);
    *pdu_text++ = TmpChars[0];
    *pdu_text++ = TmpChars[1];
  }

  *pdu_text++ = 0x1A;
  *pdu_text = 0;
  return (retval);
}

