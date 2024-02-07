#include "encoderpropertiesdialog.h"

#include "ui_propertiesdialog.h"

/**
 * @brief EncoderPropertiesDialog::EncoderPropertiesDialog
 * @param nodeInfo
 * @param plugins
 * @param parent
 */
EncoderPropertiesDialog::EncoderPropertiesDialog(std::shared_ptr<VoukoderPro::NodeInfo> nodeInfo, const std::vector<VoukoderPro::AssetInfo>& plugins, QWidget *parent):
    PropertiesDialog(nodeInfo, plugins, parent)
{
    setWindowTitle(tr("Encoder Properties"));

    const bool isVisible = nodeInfo->mediaType == VoukoderPro::MediaType::video;

    ui->tabWidget->setTabVisible(ui->tabWidget->indexOf(ui->tabStereo3D), isVisible);
    ui->tabWidget->setTabVisible(ui->tabWidget->indexOf(ui->tabSpherical), isVisible);
    ui->tabWidget->setTabVisible(ui->tabWidget->indexOf(ui->tabHDR), isVisible);
    ui->tabWidget->setTabVisible(ui->tabWidget->indexOf(ui->tabMetadata), true);

    // S3D - Type
    ui->stereo3dType->addItem("2D", "2d");
    ui->stereo3dType->addItem("Side by side", "sidebyside");
    ui->stereo3dType->addItem("Top bottom", "topbottom");
    ui->stereo3dType->addItem("Frame sequence", "framesequence");
    ui->stereo3dType->addItem("Checkerboard", "checkerboard");
    ui->stereo3dType->addItem("Side by side (Quincunx)", "sidebyside_quincunx");
    ui->stereo3dType->addItem("Lines", "lines");
    ui->stereo3dType->addItem("Columns", "columns");

    // S3D - View
    ui->stereo3dView->addItem("Packet", "packet");
    ui->stereo3dView->addItem("Left", "left");
    ui->stereo3dView->addItem("Right", "right");

    // Spherical - Projection
    ui->sphericalProjection->addItem("Equirectangular", "equirectangular");
    ui->sphericalProjection->addItem("Cubemap", "cubemap");
    ui->sphericalProjection->addItem("Equirectangular Tile", "equirectangular_tile");

    // MDD - Primaries
    ui->masteringDisplayDataPrimaries->addItem("BT.709", "bt709");
    ui->masteringDisplayDataPrimaries->addItem("DCI P3", "dcip3");
    ui->masteringDisplayDataPrimaries->addItem("BT.2020", "rec2020");

    // Meta - Languages
    ui->metaLanguages->addItem("Afar", "aar");
    ui->metaLanguages->addItem("Abkhazian", "abk");
    ui->metaLanguages->addItem("Achinese", "ace");
    ui->metaLanguages->addItem("Acoli", "ach");
    ui->metaLanguages->addItem("Adangme", "ada");
    ui->metaLanguages->addItem("Adyghe, Adygei", "ady");
    ui->metaLanguages->addItem("Afro-Asiatic languages", "afa");
    ui->metaLanguages->addItem("Afrihili", "afh");
    ui->metaLanguages->addItem("Afrikaans", "afr");
    ui->metaLanguages->addItem("Ainu", "ain");
    ui->metaLanguages->addItem("Akan", "aka");
    ui->metaLanguages->addItem("Akkadian", "akk");
    ui->metaLanguages->addItem("Aleut", "ale");
    ui->metaLanguages->addItem("Algonquian languages", "alg");
    ui->metaLanguages->addItem("Southern Altai", "alt");
    ui->metaLanguages->addItem("Amharic", "amh");
    ui->metaLanguages->addItem("English, Old (ca.450–1100)", "ang");
    ui->metaLanguages->addItem("Angika", "anp");
    ui->metaLanguages->addItem("Apache languages", "apa");
    ui->metaLanguages->addItem("Arabic", "ara");
    ui->metaLanguages->addItem("Official Aramaic (700–300 BCE), Imperial Aramaic (700–300 BCE)", "arc");
    ui->metaLanguages->addItem("Aragonese", "arg");
    ui->metaLanguages->addItem("Mapudungun, Mapuche", "arn");
    ui->metaLanguages->addItem("Arapaho", "arp");
    ui->metaLanguages->addItem("Artificial languages", "art");
    ui->metaLanguages->addItem("Arawak", "arw");
    ui->metaLanguages->addItem("Assamese", "asm");
    ui->metaLanguages->addItem("Asturian, Bable, Leonese, Asturleonese", "ast");
    ui->metaLanguages->addItem("Athapascan languages", "ath");
    ui->metaLanguages->addItem("Australian languages", "aus");
    ui->metaLanguages->addItem("Avaric", "ava");
    ui->metaLanguages->addItem("Avestan", "ave");
    ui->metaLanguages->addItem("Awadhi", "awa");
    ui->metaLanguages->addItem("Aymara", "aym");
    ui->metaLanguages->addItem("Azerbaijani", "aze");
    ui->metaLanguages->addItem("Banda languages", "bad");
    ui->metaLanguages->addItem("Bamileke languages", "bai");
    ui->metaLanguages->addItem("Bashkir", "bak");
    ui->metaLanguages->addItem("Baluchi", "bal");
    ui->metaLanguages->addItem("Bambara", "bam");
    ui->metaLanguages->addItem("Balinese", "ban");
    ui->metaLanguages->addItem("Basa", "bas");
    ui->metaLanguages->addItem("Baltic languages", "bat");
    ui->metaLanguages->addItem("Beja, Bedawiyet", "bej");
    ui->metaLanguages->addItem("Belarusian", "bel");
    ui->metaLanguages->addItem("Bemba", "bem");
    ui->metaLanguages->addItem("Bengali", "ben");
    ui->metaLanguages->addItem("Berber languages", "ber");
    ui->metaLanguages->addItem("Bhojpuri", "bho");
    ui->metaLanguages->addItem("Bihari languages", "bih");
    ui->metaLanguages->addItem("Bikol", "bik");
    ui->metaLanguages->addItem("Bini, Edo", "bin");
    ui->metaLanguages->addItem("Bislama", "bis");
    ui->metaLanguages->addItem("Siksika", "bla");
    ui->metaLanguages->addItem("Bantu languages", "bnt");
    ui->metaLanguages->addItem("Tibetan", "bod");
    ui->metaLanguages->addItem("Bosnian", "bos");
    ui->metaLanguages->addItem("Braj", "bra");
    ui->metaLanguages->addItem("Breton", "bre");
    ui->metaLanguages->addItem("Batak languages", "btk");
    ui->metaLanguages->addItem("Buriat", "bua");
    ui->metaLanguages->addItem("Buginese", "bug");
    ui->metaLanguages->addItem("Bulgarian", "bul");
    ui->metaLanguages->addItem("Blin, Bilin", "byn");
    ui->metaLanguages->addItem("Caddo", "cad");
    ui->metaLanguages->addItem("Central American Indian languages", "cai");
    ui->metaLanguages->addItem("Galibi Carib", "car");
    ui->metaLanguages->addItem("Catalan, Valencian", "cat");
    ui->metaLanguages->addItem("Caucasian languages", "cau");
    ui->metaLanguages->addItem("Cebuano", "ceb");
    ui->metaLanguages->addItem("Celtic languages", "cel");
    ui->metaLanguages->addItem("Czech", "ces");
    ui->metaLanguages->addItem("Chamorro", "cha");
    ui->metaLanguages->addItem("Chibcha", "chb");
    ui->metaLanguages->addItem("Chechen", "che");
    ui->metaLanguages->addItem("Chagatai", "chg");
    ui->metaLanguages->addItem("Chuukese", "chk");
    ui->metaLanguages->addItem("Mari", "chm");
    ui->metaLanguages->addItem("Chinook jargon", "chn");
    ui->metaLanguages->addItem("Choctaw", "cho");
    ui->metaLanguages->addItem("Chipewyan, Dene Suline", "chp");
    ui->metaLanguages->addItem("Cherokee", "chr");
    ui->metaLanguages->addItem("Church Slavic, Old Slavonic, Church Slavonic, Old Bulgarian, Old Church Slavonic", "chu");
    ui->metaLanguages->addItem("Chuvash", "chv");
    ui->metaLanguages->addItem("Cheyenne", "chy");
    ui->metaLanguages->addItem("Chamic languages", "cmc");
    ui->metaLanguages->addItem("Montenegrin", "cnr");
    ui->metaLanguages->addItem("Coptic", "cop");
    ui->metaLanguages->addItem("Cornish", "cor");
    ui->metaLanguages->addItem("Corsican", "cos");
    ui->metaLanguages->addItem("Creoles and pidgins, English based", "cpe");
    ui->metaLanguages->addItem("Creoles and pidgins, French-based", "cpf");
    ui->metaLanguages->addItem("Creoles and pidgins, Portuguese-based", "cpp");
    ui->metaLanguages->addItem("Cree", "cre");
    ui->metaLanguages->addItem("Crimean Tatar, Crimean Turkish", "crh");
    ui->metaLanguages->addItem("Creoles and pidgins", "crp");
    ui->metaLanguages->addItem("Kashubian", "csb");
    ui->metaLanguages->addItem("Cushitic languages", "cus");
    ui->metaLanguages->addItem("Welsh", "cym");
    ui->metaLanguages->addItem("Dakota", "dak");
    ui->metaLanguages->addItem("Danish", "dan");
    ui->metaLanguages->addItem("Dargwa", "dar");
    ui->metaLanguages->addItem("Land Dayak languages", "day");
    ui->metaLanguages->addItem("Delaware", "del");
    ui->metaLanguages->addItem("Slave (Athapascan)", "den");
    ui->metaLanguages->addItem("German", "deu");
    ui->metaLanguages->addItem("Dogrib", "dgr");
    ui->metaLanguages->addItem("Dinka", "din");
    ui->metaLanguages->addItem("Divehi, Dhivehi, Maldivian", "div");
    ui->metaLanguages->addItem("Dogri", "doi");
    ui->metaLanguages->addItem("Dravidian languages", "dra");
    ui->metaLanguages->addItem("Lower Sorbian", "dsb");
    ui->metaLanguages->addItem("Duala", "dua");
    ui->metaLanguages->addItem("Dutch, Middle (ca. 1050–1350)", "dum");
    ui->metaLanguages->addItem("Dyula", "dyu");
    ui->metaLanguages->addItem("Dzongkha", "dzo");
    ui->metaLanguages->addItem("Efik", "efi");
    ui->metaLanguages->addItem("Egyptian (Ancient)", "egy");
    ui->metaLanguages->addItem("Ekajuk", "eka");
    ui->metaLanguages->addItem("Greek, Modern (1453–)", "ell");
    ui->metaLanguages->addItem("Elamite", "elx");
    ui->metaLanguages->addItem("English", "eng");
    ui->metaLanguages->addItem("English, Middle (1100–1500)", "enm");
    ui->metaLanguages->addItem("Esperanto", "epo");
    ui->metaLanguages->addItem("Estonian", "est");
    ui->metaLanguages->addItem("Basque", "eus");
    ui->metaLanguages->addItem("Ewe", "ewe");
    ui->metaLanguages->addItem("Ewondo", "ewo");
    ui->metaLanguages->addItem("Fang", "fan");
    ui->metaLanguages->addItem("Faroese", "fao");
    ui->metaLanguages->addItem("Persian", "fas");
    ui->metaLanguages->addItem("Fanti", "fat");
    ui->metaLanguages->addItem("Fijian", "fij");
    ui->metaLanguages->addItem("Filipino, Pilipino", "fil");
    ui->metaLanguages->addItem("Finnish", "fin");
    ui->metaLanguages->addItem("Finno-Ugrian languages", "fiu");
    ui->metaLanguages->addItem("Fon", "fon");
    ui->metaLanguages->addItem("French", "fra");
    ui->metaLanguages->addItem("French, Middle (ca. 1400–1600)", "frm");
    ui->metaLanguages->addItem("French, Old (842–ca. 1400)", "fro");
    ui->metaLanguages->addItem("Northern Frisian", "frr");
    ui->metaLanguages->addItem("East Frisian Low Saxon", "frs");
    ui->metaLanguages->addItem("Western Frisian", "fry");
    ui->metaLanguages->addItem("Fulah", "ful");
    ui->metaLanguages->addItem("Friulian", "fur");
    ui->metaLanguages->addItem("Ga", "gaa");
    ui->metaLanguages->addItem("Gayo", "gay");
    ui->metaLanguages->addItem("Gbaya", "gba");
    ui->metaLanguages->addItem("Germanic languages", "gem");
    ui->metaLanguages->addItem("Geez", "gez");
    ui->metaLanguages->addItem("Gilbertese", "gil");
    ui->metaLanguages->addItem("Gaelic, Scottish Gaelic", "gla");
    ui->metaLanguages->addItem("Irish", "gle");
    ui->metaLanguages->addItem("Galician", "glg");
    ui->metaLanguages->addItem("Manx", "glv");
    ui->metaLanguages->addItem("German, Middle High (ca. 1050–1500)", "gmh");
    ui->metaLanguages->addItem("German, Old High (ca. 750–1050)", "goh");
    ui->metaLanguages->addItem("Gondi", "gon");
    ui->metaLanguages->addItem("Gorontalo", "gor");
    ui->metaLanguages->addItem("Gothic", "got");
    ui->metaLanguages->addItem("Grebo", "grb");
    ui->metaLanguages->addItem("Greek, Ancient (to 1453)", "grc");
    ui->metaLanguages->addItem("Guarani", "grn");
    ui->metaLanguages->addItem("Swiss German, Alemannic, Alsatian", "gsw");
    ui->metaLanguages->addItem("Gujarati", "guj");
    ui->metaLanguages->addItem("Gwich'in", "gwi");
    ui->metaLanguages->addItem("Haida", "hai");
    ui->metaLanguages->addItem("Haitian, Haitian Creole", "hat");
    ui->metaLanguages->addItem("Hausa", "hau");
    ui->metaLanguages->addItem("Hawaiian", "haw");
    ui->metaLanguages->addItem("Hebrew", "heb");
    ui->metaLanguages->addItem("Herero", "her");
    ui->metaLanguages->addItem("Hiligaynon", "hil");
    ui->metaLanguages->addItem("Himachali languages, Pahari languages", "him");
    ui->metaLanguages->addItem("Hindi", "hin");
    ui->metaLanguages->addItem("Hittite", "hit");
    ui->metaLanguages->addItem("Hmong, Mong", "hmn");
    ui->metaLanguages->addItem("Hiri Motu", "hmo");
    ui->metaLanguages->addItem("Croatian", "hrv");
    ui->metaLanguages->addItem("Upper Sorbian", "hsb");
    ui->metaLanguages->addItem("Hungarian", "hun");
    ui->metaLanguages->addItem("Hupa", "hup");
    ui->metaLanguages->addItem("Armenian", "hye");
    ui->metaLanguages->addItem("Iban", "iba");
    ui->metaLanguages->addItem("Igbo", "ibo");
    ui->metaLanguages->addItem("Ido", "ido");
    ui->metaLanguages->addItem("Sichuan Yi, Nuosu", "iii");
    ui->metaLanguages->addItem("Ijo languages", "ijo");
    ui->metaLanguages->addItem("Inuktitut", "iku");
    ui->metaLanguages->addItem("Interlingue, Occidental", "ile");
    ui->metaLanguages->addItem("Iloko", "ilo");
    ui->metaLanguages->addItem("Interlingua (International Auxiliary Language Association)", "ina");
    ui->metaLanguages->addItem("Indo-Aryan languages", "inc");
    ui->metaLanguages->addItem("Indonesian", "ind");
    ui->metaLanguages->addItem("Indo-European languages", "ine");
    ui->metaLanguages->addItem("Ingush", "inh");
    ui->metaLanguages->addItem("Inupiaq", "ipk");
    ui->metaLanguages->addItem("Iranian languages", "ira");
    ui->metaLanguages->addItem("Iroquoian languages", "iro");
    ui->metaLanguages->addItem("Icelandic", "isl");
    ui->metaLanguages->addItem("Italian", "ita");
    ui->metaLanguages->addItem("Javanese", "jav");
    ui->metaLanguages->addItem("Lojban", "jbo");
    ui->metaLanguages->addItem("Japanese", "jpn");
    ui->metaLanguages->addItem("Judeo-Persian", "jpr");
    ui->metaLanguages->addItem("Judeo-Arabic", "jrb");
    ui->metaLanguages->addItem("Kara-Kalpak", "kaa");
    ui->metaLanguages->addItem("Kabyle", "kab");
    ui->metaLanguages->addItem("Kachin, Jingpho", "kac");
    ui->metaLanguages->addItem("Kalaallisut, Greenlandic", "kal");
    ui->metaLanguages->addItem("Kamba", "kam");
    ui->metaLanguages->addItem("Kannada", "kan");
    ui->metaLanguages->addItem("Karen languages", "kar");
    ui->metaLanguages->addItem("Kashmiri", "kas");
    ui->metaLanguages->addItem("Georgian", "kat");
    ui->metaLanguages->addItem("Kanuri", "kau");
    ui->metaLanguages->addItem("Kawi", "kaw");
    ui->metaLanguages->addItem("Kazakh", "kaz");
    ui->metaLanguages->addItem("Kabardian", "kbd");
    ui->metaLanguages->addItem("Khasi", "kha");
    ui->metaLanguages->addItem("Khoisan languages", "khi");
    ui->metaLanguages->addItem("Central Khmer", "khm");
    ui->metaLanguages->addItem("Khotanese, Sakan", "kho");
    ui->metaLanguages->addItem("Kikuyu, Gikuyu", "kik");
    ui->metaLanguages->addItem("Kinyarwanda", "kin");
    ui->metaLanguages->addItem("Kirghiz, Kyrgyz", "kir");
    ui->metaLanguages->addItem("Kimbundu", "kmb");
    ui->metaLanguages->addItem("Konkani", "kok");
    ui->metaLanguages->addItem("Komi", "kom");
    ui->metaLanguages->addItem("Kongo", "kon");
    ui->metaLanguages->addItem("Korean", "kor");
    ui->metaLanguages->addItem("Kosraean", "kos");
    ui->metaLanguages->addItem("Kpelle", "kpe");
    ui->metaLanguages->addItem("Karachay-Balkar", "krc");
    ui->metaLanguages->addItem("Karelian", "krl");
    ui->metaLanguages->addItem("Kru languages", "kro");
    ui->metaLanguages->addItem("Kurukh", "kru");
    ui->metaLanguages->addItem("Kuanyama, Kwanyama", "kua");
    ui->metaLanguages->addItem("Kumyk", "kum");
    ui->metaLanguages->addItem("Kurdish", "kur");
    ui->metaLanguages->addItem("Kutenai", "kut");
    ui->metaLanguages->addItem("Ladino", "lad");
    ui->metaLanguages->addItem("Lahnda", "lah");
    ui->metaLanguages->addItem("Lamba", "lam");
    ui->metaLanguages->addItem("Lao", "lao");
    ui->metaLanguages->addItem("Latin", "lat");
    ui->metaLanguages->addItem("Latvian", "lav");
    ui->metaLanguages->addItem("Lezghian", "lez");
    ui->metaLanguages->addItem("Limburgan, Limburger, Limburgish", "lim");
    ui->metaLanguages->addItem("Lingala", "lin");
    ui->metaLanguages->addItem("Lithuanian", "lit");
    ui->metaLanguages->addItem("Mongo", "lol");
    ui->metaLanguages->addItem("Lozi", "loz");
    ui->metaLanguages->addItem("Luxembourgish, Letzeburgesch", "ltz");
    ui->metaLanguages->addItem("Luba-Lulua", "lua");
    ui->metaLanguages->addItem("Luba-Katanga", "lub");
    ui->metaLanguages->addItem("Ganda", "lug");
    ui->metaLanguages->addItem("Luiseno", "lui");
    ui->metaLanguages->addItem("Lunda", "lun");
    ui->metaLanguages->addItem("Luo (Kenya and Tanzania)", "luo");
    ui->metaLanguages->addItem("Lushai", "lus");
    ui->metaLanguages->addItem("Madurese", "mad");
    ui->metaLanguages->addItem("Magahi", "mag");
    ui->metaLanguages->addItem("Marshallese", "mah");
    ui->metaLanguages->addItem("Maithili", "mai");
    ui->metaLanguages->addItem("Makasar", "mak");
    ui->metaLanguages->addItem("Malayalam", "mal");
    ui->metaLanguages->addItem("Mandingo", "man");
    ui->metaLanguages->addItem("Austronesian languages", "map");
    ui->metaLanguages->addItem("Marathi", "mar");
    ui->metaLanguages->addItem("Masai", "mas");
    ui->metaLanguages->addItem("Moksha", "mdf");
    ui->metaLanguages->addItem("Mandar", "mdr");
    ui->metaLanguages->addItem("Mende", "men");
    ui->metaLanguages->addItem("Irish, Middle (900–1200)", "mga");
    ui->metaLanguages->addItem("Mi'kmaq, Micmac", "mic");
    ui->metaLanguages->addItem("Minangkabau", "min");
    ui->metaLanguages->addItem("Uncoded languages", "mis");
    ui->metaLanguages->addItem("Macedonian", "mkd");
    ui->metaLanguages->addItem("Mon-Khmer languages", "mkh");
    ui->metaLanguages->addItem("Malagasy", "mlg");
    ui->metaLanguages->addItem("Maltese", "mlt");
    ui->metaLanguages->addItem("Manchu", "mnc");
    ui->metaLanguages->addItem("Manipuri", "mni");
    ui->metaLanguages->addItem("Manobo languages", "mno");
    ui->metaLanguages->addItem("Mohawk", "moh");
    ui->metaLanguages->addItem("Mongolian", "mon");
    ui->metaLanguages->addItem("Mossi", "mos");
    ui->metaLanguages->addItem("M?ori", "mri");
    ui->metaLanguages->addItem("Malay", "msa");
    ui->metaLanguages->addItem("Multiple languages", "mul");
    ui->metaLanguages->addItem("Munda languages", "mun");
    ui->metaLanguages->addItem("Creek", "mus");
    ui->metaLanguages->addItem("Mirandese", "mwl");
    ui->metaLanguages->addItem("Marwari", "mwr");
    ui->metaLanguages->addItem("Burmese", "mya");
    ui->metaLanguages->addItem("Mayan languages", "myn");
    ui->metaLanguages->addItem("Erzya", "myv");
    ui->metaLanguages->addItem("Nahuatl languages", "nah");
    ui->metaLanguages->addItem("North American Indian languages", "nai");
    ui->metaLanguages->addItem("Neapolitan", "nap");
    ui->metaLanguages->addItem("Nauru", "nau");
    ui->metaLanguages->addItem("Navajo, Navaho", "nav");
    ui->metaLanguages->addItem("Ndebele, South, South Ndebele", "nbl");
    ui->metaLanguages->addItem("Ndebele, North, North Ndebele", "nde");
    ui->metaLanguages->addItem("Ndonga", "ndo");
    ui->metaLanguages->addItem("Low German, Low Saxon, German", "nds");
    ui->metaLanguages->addItem("Nepali", "nep");
    ui->metaLanguages->addItem("Nepal Bhasa, Newari", "new");
    ui->metaLanguages->addItem("Nias", "nia");
    ui->metaLanguages->addItem("Niger-Kordofanian languages", "nic");
    ui->metaLanguages->addItem("Niuean", "niu");
    ui->metaLanguages->addItem("Dutch, Flemish", "nld");
    ui->metaLanguages->addItem("Norwegian Nynorsk, Nynorsk, Norwegian", "nno");
    ui->metaLanguages->addItem("Bokmål, Norwegian, Norwegian Bokmål", "nob");
    ui->metaLanguages->addItem("Nogai", "nog");
    ui->metaLanguages->addItem("Norse, Old", "non");
    ui->metaLanguages->addItem("Norwegian", "nor");
    ui->metaLanguages->addItem("N'Ko", "nqo");
    ui->metaLanguages->addItem("Pedi, Sepedi, Northern Sotho", "nso");
    ui->metaLanguages->addItem("Nubian languages", "nub");
    ui->metaLanguages->addItem("Classical Newari, Old Newari, Classical Nepal Bhasa", "nwc");
    ui->metaLanguages->addItem("Chichewa, Chewa, Nyanja", "nya");
    ui->metaLanguages->addItem("Nyamwezi", "nym");
    ui->metaLanguages->addItem("Nyankole", "nyn");
    ui->metaLanguages->addItem("Nyoro", "nyo");
    ui->metaLanguages->addItem("Nzima", "nzi");
    ui->metaLanguages->addItem("Occitan (post 1500)", "oci");
    ui->metaLanguages->addItem("Ojibwa", "oji");
    ui->metaLanguages->addItem("Oriya", "ori");
    ui->metaLanguages->addItem("Oromo", "orm");
    ui->metaLanguages->addItem("Osage", "osa");
    ui->metaLanguages->addItem("Ossetian, Ossetic", "oss");
    ui->metaLanguages->addItem("Turkish, Ottoman (1500–1928)", "ota");
    ui->metaLanguages->addItem("Otomian languages", "oto");
    ui->metaLanguages->addItem("Papuan languages", "paa");
    ui->metaLanguages->addItem("Pangasinan", "pag");
    ui->metaLanguages->addItem("Pahlavi", "pal");
    ui->metaLanguages->addItem("Pampanga, Kapampangan", "pam");
    ui->metaLanguages->addItem("Panjabi, Punjabi", "pan");
    ui->metaLanguages->addItem("Papiamento", "pap");
    ui->metaLanguages->addItem("Palauan", "pau");
    ui->metaLanguages->addItem("Persian, Old (ca. 600–400 B.C.)", "peo");
    ui->metaLanguages->addItem("Philippine languages", "phi");
    ui->metaLanguages->addItem("Phoenician", "phn");
    ui->metaLanguages->addItem("Pali", "pli");
    ui->metaLanguages->addItem("Polish", "pol");
    ui->metaLanguages->addItem("Pohnpeian", "pon");
    ui->metaLanguages->addItem("Portuguese", "por");
    ui->metaLanguages->addItem("Prakrit languages", "pra");
    ui->metaLanguages->addItem("Provençal, Old (to 1500), Old Occitan (to 1500)", "pro");
    ui->metaLanguages->addItem("Pushto, Pashto", "pus");
    ui->metaLanguages->addItem("Quechua", "que");
    ui->metaLanguages->addItem("Rajasthani", "raj");
    ui->metaLanguages->addItem("Rapanui", "rap");
    ui->metaLanguages->addItem("Rarotongan, Cook Islands Maori", "rar");
    ui->metaLanguages->addItem("Romance languages", "roa");
    ui->metaLanguages->addItem("Romansh", "roh");
    ui->metaLanguages->addItem("Romany", "rom");
    ui->metaLanguages->addItem("Romanian, Moldavian, Moldovan", "ron");
    ui->metaLanguages->addItem("Rundi", "run");
    ui->metaLanguages->addItem("Aromanian, Arumanian, Macedo-Romanian", "rup");
    ui->metaLanguages->addItem("Russian", "rus");
    ui->metaLanguages->addItem("Sandawe", "sad");
    ui->metaLanguages->addItem("Sango", "sag");
    ui->metaLanguages->addItem("Yakut", "sah");
    ui->metaLanguages->addItem("South American Indian languages", "sai");
    ui->metaLanguages->addItem("Salishan languages", "sal");
    ui->metaLanguages->addItem("Samaritan Aramaic", "sam");
    ui->metaLanguages->addItem("Sanskrit", "san");
    ui->metaLanguages->addItem("Sasak", "sas");
    ui->metaLanguages->addItem("Santali", "sat");
    ui->metaLanguages->addItem("Sicilian", "scn");
    ui->metaLanguages->addItem("Scots", "sco");
    ui->metaLanguages->addItem("Selkup", "sel");
    ui->metaLanguages->addItem("Semitic languages", "sem");
    ui->metaLanguages->addItem("Irish, Old (to 900)", "sga");
    ui->metaLanguages->addItem("Sign Languages", "sgn");
    ui->metaLanguages->addItem("Shan", "shn");
    ui->metaLanguages->addItem("Sidamo", "sid");
    ui->metaLanguages->addItem("Sinhala, Sinhalese", "sin");
    ui->metaLanguages->addItem("Siouan languages", "sio");
    ui->metaLanguages->addItem("Sino-Tibetan languages", "sit");
    ui->metaLanguages->addItem("Slavic languages", "sla");
    ui->metaLanguages->addItem("Slovak", "slk");
    ui->metaLanguages->addItem("Slovenian", "slv");
    ui->metaLanguages->addItem("Southern Sami", "sma");
    ui->metaLanguages->addItem("Northern Sami", "sme");
    ui->metaLanguages->addItem("Sami languages", "smi");
    ui->metaLanguages->addItem("Lule Sami", "smj");
    ui->metaLanguages->addItem("Inari Sami", "smn");
    ui->metaLanguages->addItem("Samoan", "smo");
    ui->metaLanguages->addItem("Skolt Sami", "sms");
    ui->metaLanguages->addItem("Shona", "sna");
    ui->metaLanguages->addItem("Sindhi", "snd");
    ui->metaLanguages->addItem("Soninke", "snk");
    ui->metaLanguages->addItem("Sogdian", "sog");
    ui->metaLanguages->addItem("Somali", "som");
    ui->metaLanguages->addItem("Songhai languages", "son");
    ui->metaLanguages->addItem("Sotho, Southern", "sot");
    ui->metaLanguages->addItem("Spanish, Castilian", "spa");
    ui->metaLanguages->addItem("Albanian", "sqi");
    ui->metaLanguages->addItem("Sardinian", "srd");
    ui->metaLanguages->addItem("Sranan Tongo", "srn");
    ui->metaLanguages->addItem("Serbian", "srp");
    ui->metaLanguages->addItem("Serer", "srr");
    ui->metaLanguages->addItem("Nilo-Saharan languages", "ssa");
    ui->metaLanguages->addItem("Swati", "ssw");
    ui->metaLanguages->addItem("Sukuma", "suk");
    ui->metaLanguages->addItem("Sundanese", "sun");
    ui->metaLanguages->addItem("Susu", "sus");
    ui->metaLanguages->addItem("Sumerian", "sux");
    ui->metaLanguages->addItem("Swahili", "swa");
    ui->metaLanguages->addItem("Swedish", "swe");
    ui->metaLanguages->addItem("Classical Syriac", "syc");
    ui->metaLanguages->addItem("Syriac", "syr");
    ui->metaLanguages->addItem("Tahitian", "tah");
    ui->metaLanguages->addItem("Tai languages", "tai");
    ui->metaLanguages->addItem("Tamil", "tam");
    ui->metaLanguages->addItem("Tatar", "tat");
    ui->metaLanguages->addItem("Telugu", "tel");
    ui->metaLanguages->addItem("Timne", "tem");
    ui->metaLanguages->addItem("Tereno", "ter");
    ui->metaLanguages->addItem("Tetum", "tet");
    ui->metaLanguages->addItem("Tajik", "tgk");
    ui->metaLanguages->addItem("Tagalog", "tgl");
    ui->metaLanguages->addItem("Thai", "tha");
    ui->metaLanguages->addItem("Tigre", "tig");
    ui->metaLanguages->addItem("Tigrinya", "tir");
    ui->metaLanguages->addItem("Tiv", "tiv");
    ui->metaLanguages->addItem("Tokelau", "tkl");
    ui->metaLanguages->addItem("Klingon, tlhIngan-Hol", "tlh");
    ui->metaLanguages->addItem("Tlingit", "tli");
    ui->metaLanguages->addItem("Tamashek", "tmh");
    ui->metaLanguages->addItem("Tonga (Nyasa)", "tog");
    ui->metaLanguages->addItem("Tonga (Tonga Islands)", "ton");
    ui->metaLanguages->addItem("Tok Pisin", "tpi");
    ui->metaLanguages->addItem("Tsimshian", "tsi");
    ui->metaLanguages->addItem("Tswana", "tsn");
    ui->metaLanguages->addItem("Tsonga", "tso");
    ui->metaLanguages->addItem("Turkmen", "tuk");
    ui->metaLanguages->addItem("Tumbuka", "tum");
    ui->metaLanguages->addItem("Tupi languages", "tup");
    ui->metaLanguages->addItem("Turkish", "tur");
    ui->metaLanguages->addItem("Altaic languages", "tut");
    ui->metaLanguages->addItem("Tuvalu", "tvl");
    ui->metaLanguages->addItem("Twi", "twi");
    ui->metaLanguages->addItem("Tuvinian", "tyv");
    ui->metaLanguages->addItem("Udmurt", "udm");
    ui->metaLanguages->addItem("Ugaritic", "uga");
    ui->metaLanguages->addItem("Uighur, Uyghur", "uig");
    ui->metaLanguages->addItem("Ukrainian", "ukr");
    ui->metaLanguages->addItem("Umbundu", "umb");
    ui->metaLanguages->addItem("Undetermined", "und");
    ui->metaLanguages->addItem("Urdu", "urd");
    ui->metaLanguages->addItem("Uzbek", "uzb");
    ui->metaLanguages->addItem("Vai", "vai");
    ui->metaLanguages->addItem("Venda", "ven");
    ui->metaLanguages->addItem("Vietnamese", "vie");
    ui->metaLanguages->addItem("Volapük", "vol");
    ui->metaLanguages->addItem("Votic", "vot");
    ui->metaLanguages->addItem("Wakashan languages", "wak");
    ui->metaLanguages->addItem("Wolaitta, Wolaytta", "wal");
    ui->metaLanguages->addItem("Waray", "war");
    ui->metaLanguages->addItem("Washo", "was");
    ui->metaLanguages->addItem("Sorbian languages", "wen");
    ui->metaLanguages->addItem("Walloon", "wln");
    ui->metaLanguages->addItem("Wolof", "wol");
    ui->metaLanguages->addItem("Kalmyk, Oirat", "xal");
    ui->metaLanguages->addItem("Xhosa", "xho");
    ui->metaLanguages->addItem("Yao", "yao");
    ui->metaLanguages->addItem("Yapese", "yap");
    ui->metaLanguages->addItem("Yiddish", "yid");
    ui->metaLanguages->addItem("Yoruba", "yor");
    ui->metaLanguages->addItem("Yupik languages", "ypk");
    ui->metaLanguages->addItem("Zapotec", "zap");
    ui->metaLanguages->addItem("Blissymbols, Blissymbolics, Bliss", "zbl");
    ui->metaLanguages->addItem("Zenaga", "zen");
    ui->metaLanguages->addItem("Standard Moroccan Tamazight", "zgh");
    ui->metaLanguages->addItem("Zhuang, Chuang", "zha");
    ui->metaLanguages->addItem("Chinese", "zho");
    ui->metaLanguages->addItem("Zande languages", "znd");
    ui->metaLanguages->addItem("Zulu", "zul");
    ui->metaLanguages->addItem("Zuni", "zun");
    ui->metaLanguages->addItem("No linguistic content, Not applicable", "zxx");
    ui->metaLanguages->addItem("Zaza, Dimili, Dimli, Kirdki, Kirmanjki, Zazaki", "zza");

    ui->metaLanguages->model()->sort(0);

    // Load any side data if present
    if (nodeInfo->data.contains("sidedata"))
        loadSideData(nodeInfo->data["sidedata"]);
}

/**
 * @brief EncoderPropertiesDialog::loadSideData
 * @param sidedata
 */
void EncoderPropertiesDialog::loadSideData(nlohmann::ordered_json& sidedata)
{
    // Stereo 3D
    if (sidedata.contains("stereo3d") && sidedata["stereo3d"].is_object())
    {
        try
        {
            const auto stereo3d = sidedata["stereo3d"];

            // Type
            const QString s3d_type = QString::fromStdString(stereo3d["type"].get<std::string>());
            const int s3d_type_idx = ui->stereo3dType->findData(s3d_type);
            ui->stereo3dType->setCurrentIndex(std::max(0, s3d_type_idx));

            // View
            const QString s3d_view = QString::fromStdString(stereo3d["view"].get<std::string>());
            const int s3d_view_idx = ui->stereo3dView->findData(s3d_view);
            ui->stereo3dView->setCurrentIndex(std::max(0, s3d_view_idx));

            ui->stereo3dGroup->setChecked(true);
        }
        catch (nlohmann::ordered_json::exception ex)
        {
        }
    }

    // Spherical
    if (sidedata.contains("spherical") && sidedata["spherical"].is_object())
    {
        try
        {
            const auto spherical = sidedata["spherical"];

            // Projection
            const QString spherical_projection = QString::fromStdString(spherical["projection"].get<std::string>());
            const int spherical_projection_idx = ui->sphericalProjection->findData(spherical_projection);
            ui->sphericalProjection->setCurrentIndex(std::max(0, spherical_projection_idx));

            // Yaw
            const int yaw = spherical["yaw"].get<int>();
            ui->sphericalYaw->setValue(yaw);

            // Pitch
            const int pitch = spherical["pitch"].get<int>();
            ui->sphericalPitch->setValue(pitch);

            // Roll
            const int roll = spherical["roll"].get<int>();
            ui->sphericalRoll->setValue(roll);

            ui->sphericalGroup->setChecked(true);
        }
        catch (nlohmann::ordered_json::exception ex)
        {
        }
    }

    // Content light levels
    if (sidedata.contains("contentlightlevels") && sidedata["contentlightlevels"].is_object())
    {
        try
        {
            const auto contentlightlevels = sidedata["contentlightlevels"];

            // Max. CLL
            const int max_cll = contentlightlevels["max_cll"].get<int>();
            ui->contentLightLevelsMaxCLL->setValue(max_cll);

            // Max. FALL
            const int max_fall = contentlightlevels["max_fall"].get<int>();
            ui->contentLightLevelsMaxFALL->setValue(max_fall);

            ui->contentLightLevelsGroup->setChecked(true);
        }
        catch (nlohmann::ordered_json::exception ex)
        {
        }
    }

    // Mastering Display Data
    if (sidedata.contains("masteringdisplaydata") && sidedata["masteringdisplaydata"].is_object())
    {
        try
        {
            const auto masteringdisplaydata = sidedata["masteringdisplaydata"];

            // Primaries
            const QString primaries = QString::fromStdString(masteringdisplaydata["primaries"].get<std::string>());
            const int primaries_idx = ui->masteringDisplayDataPrimaries->findData(primaries);
            ui->masteringDisplayDataPrimaries->setCurrentIndex(std::max(0, primaries_idx));

            // Luminance min.
            const double min = masteringdisplaydata["luminance_min"].get<double>();
            ui->masteringDisplayDataLuminanceMin->setValue(min);

            // Luminance max.
            const int max = masteringdisplaydata["luminance_max"].get<int>();
            ui->masteringDisplayDataLuminanceMax->setValue(max);

            ui->masteringDisplayDataGroup->setChecked(true);
        }
        catch (nlohmann::ordered_json::exception ex)
        {
        }
    }

    // Meta
    if (sidedata.contains("meta") && sidedata["meta"].is_object())
    {
        try
        {
            const auto meta = sidedata["meta"];

            // Language
            const QString language = QString::fromStdString(meta["language"].get<std::string>());
            const int language_idx = ui->metaLanguages->findData(language);
            ui->metaLanguages->setCurrentIndex(std::max(0, language_idx));

            ui->metaGroup->setChecked(true);
        }
        catch (nlohmann::ordered_json::exception ex)
        {
        }
    }
}

/**
 * @brief EncoderPropertiesDialog::getValues
 * @param data
 */
void EncoderPropertiesDialog::getValues(nlohmann::ordered_json& data)
{
    // Handle standard values
    PropertiesDialog::getValues(data);

    // Do we need a sidedata sub objekt?
    if (!data.contains("sidedata"))
        data["sidedata"] = nlohmann::ordered_json::object();

    auto& sidedata = data["sidedata"];

    // Stereo 3D
    if (ui->stereo3dGroup->isChecked())
    {
        sidedata["stereo3d"] = {
            { "type", ui->stereo3dType->currentData().toString().toStdString() },
            { "view", ui->stereo3dView->currentData().toString().toStdString() }
        };
    }
    else
        sidedata.erase("stereo3d");

    // Spherical
    if (ui->sphericalGroup->isChecked())
    {
        sidedata["spherical"] = {
            { "projection", ui->sphericalProjection->currentData().toString().toStdString() },
            { "yaw", ui->sphericalYaw->value() },
            { "pitch", ui->sphericalPitch->value() },
            { "roll", ui->sphericalRoll->value() }
        };
    }
    else
        sidedata.erase("spherical");

    // Mastering Display Data
    if (ui->masteringDisplayDataGroup->isChecked())
    {
        sidedata["masteringdisplaydata"] = {
            { "primaries", ui->masteringDisplayDataPrimaries->currentData().toString().toStdString() },
            { "luminance_min", ui->masteringDisplayDataLuminanceMin->value() },
            { "luminance_max", ui->masteringDisplayDataLuminanceMax->value() }
        };
    }
    else
        sidedata.erase("masteringdisplaydata");

    // Content Light Levels
    if (ui->contentLightLevelsGroup->isChecked())
    {
        sidedata["contentlightlevels"] = {
            { "max_cll", ui->contentLightLevelsMaxCLL->value() },
            { "max_fall", ui->contentLightLevelsMaxFALL->value() }
        };
    }
    else
        sidedata.erase("contentlightlevels");

    // Meta
    if (ui->metaGroup->isChecked())
    {
        sidedata["meta"] = {
            { "language", ui->metaLanguages->currentData().toString().toStdString() }
        };
    }
    else
        sidedata.erase("meta");

    // If sidedata has no elements erase it
    if (sidedata.empty())
        data.erase("sidedata");
}
