/*
 * Parser for EMV cards.
 *
 * Copyright 2023 Leptoptilos <leptoptilos@icloud.com>
 * 
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "core/string.h"
#include "furi_hal_rtc.h"
#include "nfc_supported_card_plugin.h"

#include "protocols/emv/emv.h"
#include "protocols/nfc_protocol.h"
#include <flipper_application/flipper_application.h>

#include <nfc/nfc_device.h>
#include <nfc/helpers/nfc_util.h>

#define TAG "EMV"

char* get_country_name(uint16_t country_code) {
    switch(country_code) {
    case 0x0004:
        return "AFG";
    case 0x0008:
        return "ALB";
    case 0x0010:
        return "ATA";
    case 0x0012:
        return "DZA";
    case 0x0016:
        return "ASM";
    case 0x0020:
        return "AND";
    case 0x0024:
        return "AGO";
    case 0x0028:
        return "ATG";
    case 0x0031:
        return "AZE";
    case 0x0032:
        return "ARG";
    case 0x0036:
        return "AUS";
    case 0x0040:
        return "AUT";
    case 0x0044:
        return "BHS";
    case 0x0048:
        return "BHR";
    case 0x0050:
        return "BGD";
    case 0x0051:
        return "ARM";
    case 0x0052:
        return "BRB";
    case 0x0056:
        return "BEL";
    case 0x0060:
        return "BMU";
    case 0x0064:
        return "BTN";
    case 0x0068:
        return "BOL";
    case 0x0070:
        return "BIH";
    case 0x0072:
        return "BWA";
    case 0x0074:
        return "BVT";
    case 0x0076:
        return "BRA";
    case 0x0084:
        return "BLZ";
    case 0x0086:
        return "IOT";
    case 0x0090:
        return "SLB";
    case 0x0092:
        return "VGB";
    case 0x0096:
        return "BRN";
    case 0x0100:
        return "BGR";
    case 0x0104:
        return "MMR";
    case 0x0108:
        return "BDI";
    case 0x0112:
        return "BLR";
    case 0x0116:
        return "KHM";
    case 0x0120:
        return "CMR";
    case 0x0124:
        return "CAN";
    case 0x0132:
        return "CPV";
    case 0x0136:
        return "CYM";
    case 0x0140:
        return "CAF";
    case 0x0144:
        return "LKA";
    case 0x0148:
        return "TCD";
    case 0x0152:
        return "CHL";
    case 0x0156:
        return "CHN";
    case 0x0158:
        return "TWN";
    case 0x0162:
        return "CXR";
    case 0x0166:
        return "CCK";
    case 0x0170:
        return "COL";
    case 0x0174:
        return "COM";
    case 0x0175:
        return "MYT";
    case 0x0178:
        return "COG";
    case 0x0180:
        return "COD";
    case 0x0184:
        return "COK";
    case 0x0188:
        return "CRI";
    case 0x0191:
        return "HRV";
    case 0x0192:
        return "CUB";
    case 0x0196:
        return "CYP";
    case 0x0203:
        return "CZE";
    case 0x0204:
        return "BEN";
    case 0x0208:
        return "DNK";
    case 0x0212:
        return "DMA";
    case 0x0214:
        return "DOM";
    case 0x0218:
        return "ECU";
    case 0x0222:
        return "SLV";
    case 0x0226:
        return "GNQ";
    case 0x0231:
        return "ETH";
    case 0x0232:
        return "ERI";
    case 0x0233:
        return "EST";
    case 0x0234:
        return "FRO";
    case 0x0238:
        return "FLK";
    case 0x0239:
        return "SGS";
    case 0x0242:
        return "FJI";
    case 0x0246:
        return "FIN";
    case 0x0248:
        return "ALA";
    case 0x0250:
        return "FRA";
    case 0x0254:
        return "GUF";
    case 0x0258:
        return "PYF";
    case 0x0260:
        return "ATF";
    case 0x0262:
        return "DJI";
    case 0x0266:
        return "GAB";
    case 0x0268:
        return "GEO";
    case 0x0270:
        return "GMB";
    case 0x0275:
        return "PSE";
    case 0x0276:
        return "DEU";
    case 0x0288:
        return "GHA";
    case 0x0292:
        return "GIB";
    case 0x0296:
        return "KIR";
    case 0x0300:
        return "GRC";
    case 0x0304:
        return "GRL";
    case 0x0308:
        return "GRD";
    case 0x0312:
        return "GLP";
    case 0x0316:
        return "GUM";
    case 0x0320:
        return "GTM";
    case 0x0324:
        return "GIN";
    case 0x0328:
        return "GUY";
    case 0x0332:
        return "HTI";
    case 0x0334:
        return "HMD";
    case 0x0336:
        return "VAT";
    case 0x0340:
        return "HND";
    case 0x0344:
        return "HKG";
    case 0x0348:
        return "HUN";
    case 0x0352:
        return "ISL";
    case 0x0356:
        return "IND";
    case 0x0360:
        return "IDN";
    case 0x0364:
        return "IRN";
    case 0x0368:
        return "IRQ";
    case 0x0372:
        return "IRL";
    case 0x0376:
        return "ISR";
    case 0x0380:
        return "ITA";
    case 0x0384:
        return "CIV";
    case 0x0388:
        return "JAM";
    case 0x0392:
        return "JPN";
    case 0x0398:
        return "KAZ";
    case 0x0400:
        return "JOR";
    case 0x0404:
        return "KEN";
    case 0x0408:
        return "PRK";
    case 0x0410:
        return "KOR";
    case 0x0414:
        return "KWT";
    case 0x0417:
        return "KGZ";
    case 0x0418:
        return "LAO";
    case 0x0422:
        return "LBN";
    case 0x0426:
        return "LSO";
    case 0x0428:
        return "LVA";
    case 0x0430:
        return "LBR";
    case 0x0434:
        return "LBY";
    case 0x0438:
        return "LIE";
    case 0x0440:
        return "LTU";
    case 0x0442:
        return "LUX";
    case 0x0446:
        return "MAC";
    case 0x0450:
        return "MDG";
    case 0x0454:
        return "MWI";
    case 0x0458:
        return "MYS";
    case 0x0462:
        return "MDV";
    case 0x0466:
        return "MLI";
    case 0x0470:
        return "MLT";
    case 0x0474:
        return "MTQ";
    case 0x0478:
        return "MRT";
    case 0x0480:
        return "MUS";
    case 0x0484:
        return "MEX";
    case 0x0492:
        return "MCO";
    case 0x0496:
        return "MNG";
    case 0x0498:
        return "MDA";
    case 0x0499:
        return "MNE";
    case 0x0500:
        return "MSR";
    case 0x0504:
        return "MAR";
    case 0x0508:
        return "MOZ";
    case 0x0512:
        return "OMN";
    case 0x0516:
        return "NAM";
    case 0x0520:
        return "NRU";
    case 0x0524:
        return "NPL";
    case 0x0528:
        return "NLD";
    case 0x0531:
        return "CUW";
    case 0x0533:
        return "ABW";
    case 0x0534:
        return "SXM";
    case 0x0535:
        return "BES";
    case 0x0540:
        return "NCL";
    case 0x0548:
        return "VUT";
    case 0x0554:
        return "NZL";
    case 0x0558:
        return "NIC";
    case 0x0562:
        return "NER";
    case 0x0566:
        return "NGA";
    case 0x0570:
        return "NIU";
    case 0x0574:
        return "NFK";
    case 0x0578:
        return "NOR";
    case 0x0580:
        return "MNP";
    case 0x0581:
        return "UMI";
    case 0x0583:
        return "FSM";
    case 0x0584:
        return "MHL";
    case 0x0585:
        return "PLW";
    case 0x0586:
        return "PAK";
    case 0x0591:
        return "PAN";
    case 0x0598:
        return "PNG";
    case 0x0600:
        return "PRY";
    case 0x0604:
        return "PER";
    case 0x0608:
        return "PHL";
    case 0x0612:
        return "PCN";
    case 0x0616:
        return "POL";
    case 0x0620:
        return "PRT";
    case 0x0624:
        return "GNB";
    case 0x0626:
        return "TLS";
    case 0x0630:
        return "PRI";
    case 0x0634:
        return "QAT";
    case 0x0638:
        return "REU";
    case 0x0642:
        return "ROU";
    case 0x0643:
        return "RUS";
    case 0x0646:
        return "RWA";
    case 0x0652:
        return "BLM";
    case 0x0654:
        return "SHN";
    case 0x0659:
        return "KNA";
    case 0x0660:
        return "AIA";
    case 0x0662:
        return "LCA";
    case 0x0663:
        return "MAF";
    case 0x0666:
        return "SPM";
    case 0x0670:
        return "VCT";
    case 0x0674:
        return "SMR";
    case 0x0678:
        return "STP";
    case 0x0682:
        return "SAU";
    case 0x0686:
        return "SEN";
    case 0x0688:
        return "SRB";
    case 0x0690:
        return "SYC";
    case 0x0694:
        return "SLE";
    case 0x0702:
        return "SGP";
    case 0x0703:
        return "SVK";
    case 0x0704:
        return "VNM";
    case 0x0705:
        return "SVN";
    case 0x0706:
        return "SOM";
    case 0x0710:
        return "ZAF";
    case 0x0716:
        return "ZWE";
    case 0x0724:
        return "ESP";
    case 0x0728:
        return "SSD";
    case 0x0729:
        return "SDN";
    case 0x0732:
        return "ESH";
    case 0x0740:
        return "SUR";
    case 0x0744:
        return "SJM";
    case 0x0748:
        return "SWZ";
    case 0x0752:
        return "SWE";
    case 0x0756:
        return "CHE";
    case 0x0760:
        return "SYR";
    case 0x0762:
        return "TJK";
    case 0x0764:
        return "THA";
    case 0x0768:
        return "TGO";
    case 0x0772:
        return "TKL";
    case 0x0776:
        return "TON";
    case 0x0780:
        return "TTO";
    case 0x0784:
        return "ARE";
    case 0x0788:
        return "TUN";
    case 0x0792:
        return "TUR";
    case 0x0795:
        return "TKM";
    case 0x0796:
        return "TCA";
    case 0x0798:
        return "TUV";
    case 0x0800:
        return "UGA";
    case 0x0804:
        return "UKR";
    case 0x0807:
        return "MKD";
    case 0x0818:
        return "EGY";
    case 0x0826:
        return "GBR";
    case 0x0831:
        return "GGY";
    case 0x0832:
        return "JEY";
    case 0x0833:
        return "IMN";
    case 0x0834:
        return "TZA";
    case 0x0840:
        return "USA";
    case 0x0850:
        return "VIR";
    case 0x0854:
        return "BFA";
    case 0x0858:
        return "URY";
    case 0x0860:
        return "UZB";
    case 0x0862:
        return "VEN";
    case 0x0876:
        return "WLF";
    case 0x0882:
        return "WSM";
    case 0x0887:
        return "YEM";
    case 0x0894:
        return "ZMB";
    default:
        return "UNKNOWN";
    }
}

char* get_currency_name(uint16_t currency_code) {
    switch(currency_code) {
    case 0x0997:
        return "USN";
    case 0x0994:
        return "XSU";
    case 0x0990:
        return "CLF";
    case 0x0986:
        return "BRL";
    case 0x0985:
        return "PLN";
    case 0x0984:
        return "BOV";
    case 0x0981:
        return "GEL";
    case 0x0980:
        return "UAH";
    case 0x0979:
        return "MXV";
    case 0x0978:
        return "EUR";
    case 0x0977:
        return "BAM";
    case 0x0976:
        return "CDF";
    case 0x0975:
        return "BGN";
    case 0x0973:
        return "AOA";
    case 0x0972:
        return "TJS";
    case 0x0971:
        return "AFN";
    case 0x0970:
        return "COU";
    case 0x0969:
        return "MGA";
    case 0x0968:
        return "SRD";
    case 0x0967:
        return "ZMW";
    case 0x0965:
        return "XUA";
    case 0x0960:
        return "XDR";
    case 0x0953:
        return "XPF";
    case 0x0952:
        return "XOF";
    case 0x0951:
        return "XCD";
    case 0x0950:
        return "XAF";
    case 0x0949:
        return "TRY";
    case 0x0948:
        return "CHW";
    case 0x0947:
        return "CHE";
    case 0x0946:
        return "RON";
    case 0x0944:
        return "AZN";
    case 0x0943:
        return "MZN";
    case 0x0941:
        return "RSD";
    case 0x0940:
        return "UYI";
    case 0x0938:
        return "SDG";
    case 0x0937:
        return "VEF";
    case 0x0936:
        return "GHS";
    case 0x0934:
        return "TMT";
    case 0x0933:
        return "BYN";
    case 0x0932:
        return "ZWL";
    case 0x0931:
        return "CUC";
    case 0x0930:
        return "STN";
    case 0x0929:
        return "MRU";
    case 0x0901:
        return "TWD";
    case 0x0886:
        return "YER";
    case 0x0882:
        return "WST";
    case 0x0860:
        return "UZS";
    case 0x0858:
        return "UYU";
    case 0x0840:
        return "USD";
    case 0x0834:
        return "TZS";
    case 0x0826:
        return "GBP";
    case 0x0818:
        return "EGP";
    case 0x0807:
        return "MKD";
    case 0x0800:
        return "UGX";
    case 0x0788:
        return "TND";
    case 0x0784:
        return "AED";
    case 0x0780:
        return "TTD";
    case 0x0776:
        return "TOP";
    case 0x0764:
        return "THB";
    case 0x0760:
        return "SYP";
    case 0x0756:
        return "CHF";
    case 0x0752:
        return "SEK";
    case 0x0748:
        return "SZL";
    case 0x0728:
        return "SSP";
    case 0x0710:
        return "ZAR";
    case 0x0706:
        return "SOS";
    case 0x0704:
        return "VND";
    case 0x0702:
        return "SGD";
    case 0x0694:
        return "SLL";
    case 0x0690:
        return "SCR";
    case 0x0682:
        return "SAR";
    case 0x0654:
        return "SHP";
    case 0x0646:
        return "RWF";
    case 0x0643:
        return "RUB";
    case 0x0634:
        return "QAR";
    case 0x0608:
        return "PHP";
    case 0x0604:
        return "PEN";
    case 0x0600:
        return "PYG";
    case 0x0598:
        return "PGK";
    case 0x0590:
        return "PAB";
    case 0x0586:
        return "PKR";
    case 0x0578:
        return "NOK";
    case 0x0566:
        return "NGN";
    case 0x0558:
        return "NIO";
    case 0x0554:
        return "NZD";
    case 0x0548:
        return "VUV";
    case 0x0533:
        return "AWG";
    case 0x0532:
        return "ANG";
    case 0x0524:
        return "NPR";
    case 0x0516:
        return "NAD";
    case 0x0512:
        return "OMR";
    case 0x0504:
        return "MAD";
    case 0x0498:
        return "MDL";
    case 0x0496:
        return "MNT";
    case 0x0484:
        return "MXN";
    case 0x0480:
        return "MUR";
    case 0x0462:
        return "MVR";
    case 0x0458:
        return "MYR";
    case 0x0454:
        return "MWK";
    case 0x0446:
        return "MOP";
    case 0x0434:
        return "LYD";
    case 0x0430:
        return "LRD";
    case 0x0426:
        return "LSL";
    case 0x0422:
        return "LBP";
    case 0x0418:
        return "LAK";
    case 0x0417:
        return "KGS";
    case 0x0414:
        return "KWD";
    case 0x0410:
        return "KRW";
    case 0x0408:
        return "KPW";
    case 0x0404:
        return "KES";
    case 0x0400:
        return "JOD";
    case 0x0398:
        return "KZT";
    case 0x0392:
        return "JPY";
    case 0x0388:
        return "JMD";
    case 0x0376:
        return "ILS";
    case 0x0368:
        return "IQD";
    case 0x0364:
        return "IRR";
    case 0x0360:
        return "IDR";
    case 0x0356:
        return "INR";
    case 0x0352:
        return "ISK";
    case 0x0348:
        return "HUF";
    case 0x0344:
        return "HKD";
    case 0x0340:
        return "HNL";
    case 0x0332:
        return "HTG";
    case 0x0328:
        return "GYD";
    case 0x0324:
        return "GNF";
    case 0x0320:
        return "GTQ";
    case 0x0292:
        return "GIP";
    case 0x0270:
        return "GMD";
    case 0x0262:
        return "DJF";
    case 0x0242:
        return "FJD";
    case 0x0238:
        return "FKP";
    case 0x0232:
        return "ERN";
    case 0x0230:
        return "ETB";
    case 0x0222:
        return "SVC";
    case 0x0214:
        return "DOP";
    case 0x0208:
        return "DKK";
    case 0x0203:
        return "CZK";
    case 0x0192:
        return "CUP";
    case 0x0191:
        return "HRK";
    case 0x0188:
        return "CRC";
    case 0x0174:
        return "KMF";
    case 0x0170:
        return "COP";
    case 0x0156:
        return "CNY";
    case 0x0152:
        return "CLP";
    case 0x0144:
        return "LKR";
    case 0x0136:
        return "KYD";
    case 0x0132:
        return "CVE";
    case 0x0124:
        return "CAD";
    case 0x0116:
        return "KHR";
    case 0x0108:
        return "BIF";
    case 0x0104:
        return "MMK";
    case 0x0096:
        return "BND";
    case 0x0090:
        return "SBD";
    case 0x0084:
        return "BZD";
    case 0x0072:
        return "BWP";
    case 0x0068:
        return "BOB";
    case 0x0064:
        return "BTN";
    case 0x0060:
        return "BMD";
    case 0x0052:
        return "BBD";
    case 0x0051:
        return "AMD";
    case 0x0050:
        return "BDT";
    case 0x0048:
        return "BHD";
    case 0x0044:
        return "BSD";
    case 0x0036:
        return "AUD";
    case 0x0032:
        return "ARS";
    case 0x0012:
        return "DZD";
    case 0x0008:
        return "ALL";
    default:
        return "UNKNOWN";
    }
}

static bool emv_parse(const NfcDevice* device, FuriString* parsed_data) {
    furi_assert(device);
    bool parsed = false;

    const EmvData* data = nfc_device_get_data(device, NfcProtocolEmv);
    const EmvApplication app = data->emv_application;

    do {
        furi_string_cat_printf(parsed_data, "\e#AID:\n");
        for(uint8_t i = 0; i < app.aid_len; i++)
            furi_string_cat_printf(parsed_data, "%02X ", app.aid[i]);

        furi_string_cat_printf(parsed_data, "\nCountry: %s", get_country_name(app.country_code));

        furi_string_cat_printf(
            parsed_data, "\nCurrency: %s", get_currency_name(app.currency_code));

        if(app.name_found) furi_string_cat_printf(parsed_data, "\nName: %s", app.name);

        parsed = true;
    } while(false);

    return parsed;
}

/* Actual implementation of app<>plugin interface */
static const NfcSupportedCardsPlugin emv_plugin = {
    .protocol = NfcProtocolEmv,
    .verify = NULL,
    .read = NULL,
    .parse = emv_parse,
};

/* Plugin descriptor to comply with basic plugin specification */
static const FlipperAppPluginDescriptor emv_plugin_descriptor = {
    .appid = NFC_SUPPORTED_CARD_PLUGIN_APP_ID,
    .ep_api_version = NFC_SUPPORTED_CARD_PLUGIN_API_VERSION,
    .entry_point = &emv_plugin,
};

/* Plugin entry point - must return a pointer to const descriptor  */
const FlipperAppPluginDescriptor* emv_plugin_ep() {
    return &emv_plugin_descriptor;
}