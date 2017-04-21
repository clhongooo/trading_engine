#include "TestKolmogorovSmirnov.h"

TestKolmogorovSmirnov::TestKolmogorovSmirnov()
{
}

TestKolmogorovSmirnov::~TestKolmogorovSmirnov()
{
}

void TestKolmogorovSmirnov::RunTest()
{

  {
    vector<double> data1;
    data1.push_back(4 );
    data1.push_back(78);
    data1.push_back(21);
    data1.push_back(49);
    data1.push_back(34);
    data1.push_back(86);
    data1.push_back(48);
    data1.push_back(26);
    data1.push_back(16);
    data1.push_back(22);
    data1.push_back(46);
    data1.push_back(62);
    data1.push_back(4 );
    data1.push_back(16);
    data1.push_back(50);
    data1.push_back(67);
    data1.push_back(61);
    data1.push_back(59);
    data1.push_back(48);
    data1.push_back(29);
    data1.push_back(96);
    data1.push_back(64);
    data1.push_back(54);
    data1.push_back(7 );
    data1.push_back(65);
    data1.push_back(0 );
    data1.push_back(68);
    data1.push_back(17);
    data1.push_back(4 );
    data1.push_back(9 );
    data1.push_back(75);
    data1.push_back(52);
    data1.push_back(78);
    data1.push_back(11);
    data1.push_back(9 );
    data1.push_back(85);
    data1.push_back(87);
    data1.push_back(22);
    data1.push_back(70);
    data1.push_back(85);
    data1.push_back(34);
    data1.push_back(97);
    data1.push_back(92);
    data1.push_back(45);
    data1.push_back(63);
    data1.push_back(11);
    data1.push_back(96);
    data1.push_back(24);
    data1.push_back(82);
    data1.push_back(25);
    data1.push_back(42);
    data1.push_back(82);
    data1.push_back(90);
    data1.push_back(66);
    data1.push_back(16);
    data1.push_back(50);
    data1.push_back(19);
    data1.push_back(24);
    data1.push_back(51);
    data1.push_back(57);
    data1.push_back(88);
    data1.push_back(94);
    data1.push_back(64);
    data1.push_back(87);
    data1.push_back(42);
    data1.push_back(9 );
    data1.push_back(61);
    data1.push_back(8 );
    data1.push_back(64);
    data1.push_back(47);
    data1.push_back(8 );
    data1.push_back(57);
    data1.push_back(71);
    data1.push_back(8 );
    data1.push_back(32);
    data1.push_back(2 );
    data1.push_back(86);
    data1.push_back(9 );
    data1.push_back(63);
    data1.push_back(38);
    data1.push_back(68);
    data1.push_back(74);
    data1.push_back(70);
    data1.push_back(82);
    data1.push_back(98);
    data1.push_back(48);
    data1.push_back(30);
    data1.push_back(55);
    data1.push_back(41);
    data1.push_back(4 );
    data1.push_back(22);
    data1.push_back(19);
    data1.push_back(12);
    data1.push_back(40);
    data1.push_back(65);
    data1.push_back(57);
    data1.push_back(84);
    data1.push_back(10);
    data1.push_back(15);
    data1.push_back(5 );
    data1.push_back(3 );
    data1.push_back(28);
    data1.push_back(80);
    data1.push_back(33);
    data1.push_back(13);
    data1.push_back(47);
    data1.push_back(88);
    data1.push_back(93);
    data1.push_back(84);
    data1.push_back(81);
    data1.push_back(24);
    data1.push_back(62);
    data1.push_back(10);
    data1.push_back(27);
    data1.push_back(96);
    data1.push_back(90);
    data1.push_back(91);
    data1.push_back(75);
    data1.push_back(1 );
    data1.push_back(31);
    data1.push_back(52);
    data1.push_back(88);
    data1.push_back(83);
    data1.push_back(34);
    data1.push_back(38);
    data1.push_back(54);
    data1.push_back(32);
    data1.push_back(49);
    data1.push_back(10);
    data1.push_back(12);
    data1.push_back(60);
    data1.push_back(50);
    data1.push_back(93);
    data1.push_back(16);
    data1.push_back(36);
    data1.push_back(87);
    data1.push_back(85);
    data1.push_back(44);
    data1.push_back(0 );
    data1.push_back(70);
    data1.push_back(81);
    data1.push_back(66);
    data1.push_back(72);
    data1.push_back(63);
    data1.push_back(24);
    data1.push_back(90);
    data1.push_back(70);
    data1.push_back(80);
    data1.push_back(68);
    data1.push_back(47);
    data1.push_back(68);
    data1.push_back(59);
    data1.push_back(21);
    data1.push_back(91);
    data1.push_back(22);
    data1.push_back(96);
    data1.push_back(99);
    data1.push_back(22);
    data1.push_back(94);
    data1.push_back(82);
    data1.push_back(91);
    data1.push_back(60);
    data1.push_back(36);
    data1.push_back(87);
    data1.push_back(47);
    data1.push_back(87);
    data1.push_back(91);
    data1.push_back(49);
    data1.push_back(55);
    data1.push_back(43);
    data1.push_back(23);
    data1.push_back(75);
    data1.push_back(19);
    data1.push_back(45);
    data1.push_back(45);
    data1.push_back(83);
    data1.push_back(38);
    data1.push_back(5 );
    data1.push_back(24);
    data1.push_back(15);
    data1.push_back(22);
    data1.push_back(82);
    data1.push_back(67);
    data1.push_back(95);
    data1.push_back(75);
    data1.push_back(78);
    data1.push_back(72);
    data1.push_back(7 );
    data1.push_back(85);
    data1.push_back(4 );
    data1.push_back(46);
    data1.push_back(54);
    data1.push_back(16);
    data1.push_back(2 );
    data1.push_back(36);
    data1.push_back(7 );
    data1.push_back(50);
    data1.push_back(74);
    data1.push_back(23);
    data1.push_back(98);

    vector<double> data2;
    data2.push_back(43);
    data2.push_back(20);
    data2.push_back(35);
    data2.push_back(99);
    data2.push_back(2 );
    data2.push_back(46);
    data2.push_back(74);
    data2.push_back(76);
    data2.push_back(68);
    data2.push_back(18);
    data2.push_back(6 );
    data2.push_back(62);
    data2.push_back(18);
    data2.push_back(91);
    data2.push_back(20);
    data2.push_back(64);
    data2.push_back(67);
    data2.push_back(94);
    data2.push_back(88);
    data2.push_back(80);
    data2.push_back(93);
    data2.push_back(6 );
    data2.push_back(5 );
    data2.push_back(78);
    data2.push_back(27);
    data2.push_back(91);
    data2.push_back(76);
    data2.push_back(88);
    data2.push_back(8 );
    data2.push_back(24);
    data2.push_back(50);
    data2.push_back(25);
    data2.push_back(95);
    data2.push_back(57);
    data2.push_back(72);
    data2.push_back(65);
    data2.push_back(99);
    data2.push_back(38);
    data2.push_back(85);
    data2.push_back(53);
    data2.push_back(63);
    data2.push_back(23);
    data2.push_back(97);
    data2.push_back(90);
    data2.push_back(46);
    data2.push_back(21);
    data2.push_back(8 );
    data2.push_back(51);
    data2.push_back(82);
    data2.push_back(19);
    data2.push_back(26);
    data2.push_back(1 );
    data2.push_back(49);
    data2.push_back(1 );
    data2.push_back(95);
    data2.push_back(19);
    data2.push_back(71);
    data2.push_back(17);
    data2.push_back(71);
    data2.push_back(37);
    data2.push_back(82);
    data2.push_back(22);
    data2.push_back(54);
    data2.push_back(30);
    data2.push_back(6 );
    data2.push_back(99);
    data2.push_back(87);
    data2.push_back(16);
    data2.push_back(3 );
    data2.push_back(83);
    data2.push_back(99);
    data2.push_back(56);
    data2.push_back(7 );
    data2.push_back(24);
    data2.push_back(34);
    data2.push_back(11);
    data2.push_back(79);
    data2.push_back(39);
    data2.push_back(25);
    data2.push_back(8 );
    data2.push_back(32);
    data2.push_back(23);
    data2.push_back(88);
    data2.push_back(80);
    data2.push_back(58);
    data2.push_back(92);
    data2.push_back(98);
    data2.push_back(96);
    data2.push_back(42);
    data2.push_back(46);
    data2.push_back(84);
    data2.push_back(20);
    data2.push_back(30);
    data2.push_back(30);
    data2.push_back(31);
    data2.push_back(64);
    data2.push_back(38);
    data2.push_back(53);
    data2.push_back(54);
    data2.push_back(28);
    data2.push_back(79);
    data2.push_back(77);
    data2.push_back(99);
    data2.push_back(66);
    data2.push_back(2 );
    data2.push_back(90);
    data2.push_back(43);
    data2.push_back(14);
    data2.push_back(76);
    data2.push_back(36);
    data2.push_back(12);
    data2.push_back(64);
    data2.push_back(7 );
    data2.push_back(34);
    data2.push_back(68);
    data2.push_back(39);
    data2.push_back(25);
    data2.push_back(7 );
    data2.push_back(71);
    data2.push_back(88);
    data2.push_back(35);
    data2.push_back(74);
    data2.push_back(38);
    data2.push_back(27);
    data2.push_back(34);
    data2.push_back(96);
    data2.push_back(25);
    data2.push_back(22);
    data2.push_back(3 );
    data2.push_back(25);
    data2.push_back(35);
    data2.push_back(25);
    data2.push_back(15);
    data2.push_back(97);
    data2.push_back(16);
    data2.push_back(29);
    data2.push_back(97);
    data2.push_back(6 );
    data2.push_back(5 );
    data2.push_back(99);
    data2.push_back(43);
    data2.push_back(84);
    data2.push_back(20);
    data2.push_back(76);
    data2.push_back(27);
    data2.push_back(27);
    data2.push_back(48);
    data2.push_back(10);
    data2.push_back(45);
    data2.push_back(82);
    data2.push_back(2 );
    data2.push_back(10);
    data2.push_back(65);
    data2.push_back(27);
    data2.push_back(31);
    data2.push_back(79);
    data2.push_back(2 );
    data2.push_back(45);
    data2.push_back(76);
    data2.push_back(85);
    data2.push_back(66);
    data2.push_back(18);
    data2.push_back(0 );
    data2.push_back(5 );
    data2.push_back(33);
    data2.push_back(50);
    data2.push_back(13);
    data2.push_back(56);
    data2.push_back(81);
    data2.push_back(16);
    data2.push_back(98);
    data2.push_back(80);
    data2.push_back(24);
    data2.push_back(95);
    data2.push_back(90);
    data2.push_back(98);
    data2.push_back(71);
    data2.push_back(48);
    data2.push_back(87);
    data2.push_back(2 );
    data2.push_back(92);
    data2.push_back(91);
    data2.push_back(15);
    data2.push_back(96);
    data2.push_back(5 );
    data2.push_back(86);
    data2.push_back(47);
    data2.push_back(81);
    data2.push_back(82);
    data2.push_back(26);
    data2.push_back(55);
    data2.push_back(0 );
    data2.push_back(24);
    data2.push_back(88);
    data2.push_back(14);
    data2.push_back(87);
    data2.push_back(77);
    data2.push_back(6 );
    data2.push_back(20);
    data2.push_back(69);

    CPPUNIT_ASSERT(abs(KolmogorovSmirnov::TwoSmpKSStat(data1, data2, 10)-0.085) < 0.001);
  }

  {
    vector<double> data1;
    data1.push_back(81);
    data1.push_back(28);
    data1.push_back(28);
    data1.push_back(44);
    data1.push_back(0 );
    data1.push_back(17);
    data1.push_back(74);
    data1.push_back(16);
    data1.push_back(9 );
    data1.push_back(37);
    data1.push_back(63);
    data1.push_back(26);
    data1.push_back(88);
    data1.push_back(46);
    data1.push_back(4 );
    data1.push_back(22);
    data1.push_back(82);
    data1.push_back(57);
    data1.push_back(27);
    data1.push_back(50);
    data1.push_back(14);
    data1.push_back(74);
    data1.push_back(63);
    data1.push_back(50);
    data1.push_back(82);
    data1.push_back(26);
    data1.push_back(62);
    data1.push_back(82);
    data1.push_back(86);
    data1.push_back(74);
    data1.push_back(99);
    data1.push_back(52);
    data1.push_back(18);
    data1.push_back(84);
    data1.push_back(47);
    data1.push_back(38);
    data1.push_back(2 );
    data1.push_back(0 );
    data1.push_back(64);
    data1.push_back(51);
    data1.push_back(54);
    data1.push_back(36);
    data1.push_back(87);
    data1.push_back(8 );
    data1.push_back(40);
    data1.push_back(9 );
    data1.push_back(73);
    data1.push_back(86);
    data1.push_back(65);
    data1.push_back(84);
    data1.push_back(5 );
    data1.push_back(75);
    data1.push_back(87);
    data1.push_back(23);
    data1.push_back(18);
    data1.push_back(90);
    data1.push_back(26);
    data1.push_back(84);
    data1.push_back(18);
    data1.push_back(58);
    data1.push_back(21);
    data1.push_back(32);
    data1.push_back(1 );
    data1.push_back(73);
    data1.push_back(13);
    data1.push_back(30);
    data1.push_back(56);
    data1.push_back(37);
    data1.push_back(35);
    data1.push_back(72);
    data1.push_back(7 );
    data1.push_back(70);
    data1.push_back(52);
    data1.push_back(91);
    data1.push_back(80);
    data1.push_back(2 );
    data1.push_back(84);
    data1.push_back(77);
    data1.push_back(28);
    data1.push_back(72);
    data1.push_back(55);
    data1.push_back(68);
    data1.push_back(38);
    data1.push_back(42);
    data1.push_back(28);
    data1.push_back(35);
    data1.push_back(64);
    data1.push_back(53);
    data1.push_back(83);
    data1.push_back(4 );
    data1.push_back(65);
    data1.push_back(28);
    data1.push_back(65);
    data1.push_back(61);
    data1.push_back(16);
    data1.push_back(4 );
    data1.push_back(0 );
    data1.push_back(9 );
    data1.push_back(24);
    data1.push_back(90);
    data1.push_back(21);
    data1.push_back(13);
    data1.push_back(55);
    data1.push_back(56);
    data1.push_back(37);
    data1.push_back(52);
    data1.push_back(96);
    data1.push_back(90);
    data1.push_back(57);
    data1.push_back(24);
    data1.push_back(0 );
    data1.push_back(84);
    data1.push_back(78);
    data1.push_back(36);
    data1.push_back(21);
    data1.push_back(36);
    data1.push_back(48);
    data1.push_back(78);
    data1.push_back(22);
    data1.push_back(83);
    data1.push_back(68);
    data1.push_back(69);
    data1.push_back(69);
    data1.push_back(49);
    data1.push_back(22);
    data1.push_back(14);
    data1.push_back(52);
    data1.push_back(5 );
    data1.push_back(79);
    data1.push_back(3 );
    data1.push_back(8 );
    data1.push_back(9 );
    data1.push_back(62);
    data1.push_back(71);
    data1.push_back(20);
    data1.push_back(17);
    data1.push_back(90);
    data1.push_back(61);
    data1.push_back(28);
    data1.push_back(43);
    data1.push_back(17);
    data1.push_back(95);
    data1.push_back(21);
    data1.push_back(94);
    data1.push_back(72);
    data1.push_back(55);
    data1.push_back(83);
    data1.push_back(29);
    data1.push_back(28);
    data1.push_back(11);
    data1.push_back(97);
    data1.push_back(66);
    data1.push_back(84);
    data1.push_back(35);
    data1.push_back(91);
    data1.push_back(27);
    data1.push_back(44);
    data1.push_back(88);
    data1.push_back(44);
    data1.push_back(12);
    data1.push_back(4 );
    data1.push_back(13);
    data1.push_back(37);
    data1.push_back(18);
    data1.push_back(54);
    data1.push_back(74);
    data1.push_back(28);
    data1.push_back(85);
    data1.push_back(60);
    data1.push_back(36);
    data1.push_back(16);
    data1.push_back(18);
    data1.push_back(76);
    data1.push_back(98);
    data1.push_back(67);
    data1.push_back(81);
    data1.push_back(93);
    data1.push_back(31);
    data1.push_back(42);
    data1.push_back(79);
    data1.push_back(41);
    data1.push_back(35);
    data1.push_back(25);
    data1.push_back(35);
    data1.push_back(11);
    data1.push_back(13);
    data1.push_back(7 );
    data1.push_back(27);
    data1.push_back(49);
    data1.push_back(33);
    data1.push_back(54);
    data1.push_back(34);
    data1.push_back(33);
    data1.push_back(77);
    data1.push_back(85);
    data1.push_back(70);
    data1.push_back(55);
    data1.push_back(10);
    data1.push_back(1 );
    data1.push_back(75);

    vector<double> data2;
    data2.push_back(9 );
    data2.push_back(99);
    data2.push_back(53);
    data2.push_back(82);
    data2.push_back(52);
    data2.push_back(38);
    data2.push_back(94);
    data2.push_back(24);
    data2.push_back(24);
    data2.push_back(18);
    data2.push_back(68);
    data2.push_back(18);
    data2.push_back(96);
    data2.push_back(94);
    data2.push_back(57);
    data2.push_back(44);
    data2.push_back(67);
    data2.push_back(46);
    data2.push_back(68);
    data2.push_back(67);
    data2.push_back(56);
    data2.push_back(45);
    data2.push_back(79);
    data2.push_back(59);
    data2.push_back(18);
    data2.push_back(67);
    data2.push_back(0 );
    data2.push_back(32);
    data2.push_back(19);
    data2.push_back(17);
    data2.push_back(61);
    data2.push_back(39);
    data2.push_back(6 );
    data2.push_back(66);
    data2.push_back(86);
    data2.push_back(69);
    data2.push_back(44);
    data2.push_back(18);
    data2.push_back(25);
    data2.push_back(77);
    data2.push_back(48);
    data2.push_back(28);
    data2.push_back(10);
    data2.push_back(45);
    data2.push_back(71);
    data2.push_back(8 );
    data2.push_back(77);
    data2.push_back(4 );
    data2.push_back(50);
    data2.push_back(97);
    data2.push_back(91);
    data2.push_back(65);
    data2.push_back(47);
    data2.push_back(39);
    data2.push_back(70);
    data2.push_back(44);
    data2.push_back(39);
    data2.push_back(26);
    data2.push_back(11);
    data2.push_back(17);
    data2.push_back(94);
    data2.push_back(4 );
    data2.push_back(20);
    data2.push_back(71);
    data2.push_back(92);
    data2.push_back(29);
    data2.push_back(66);
    data2.push_back(86);
    data2.push_back(81);
    data2.push_back(76);
    data2.push_back(85);
    data2.push_back(46);
    data2.push_back(74);
    data2.push_back(80);
    data2.push_back(14);
    data2.push_back(3 );
    data2.push_back(19);
    data2.push_back(25);
    data2.push_back(52);
    data2.push_back(0 );
    data2.push_back(72);
    data2.push_back(55);
    data2.push_back(57);
    data2.push_back(92);
    data2.push_back(99);
    data2.push_back(98);
    data2.push_back(81);
    data2.push_back(46);
    data2.push_back(0 );
    data2.push_back(51);
    data2.push_back(83);
    data2.push_back(77);
    data2.push_back(86);
    data2.push_back(49);
    data2.push_back(87);
    data2.push_back(91);
    data2.push_back(73);
    data2.push_back(30);
    data2.push_back(27);
    data2.push_back(72);
    data2.push_back(23);
    data2.push_back(12);
    data2.push_back(22);
    data2.push_back(87);
    data2.push_back(29);
    data2.push_back(18);
    data2.push_back(58);
    data2.push_back(77);
    data2.push_back(86);
    data2.push_back(32);
    data2.push_back(93);
    data2.push_back(12);
    data2.push_back(8 );
    data2.push_back(49);
    data2.push_back(49);
    data2.push_back(69);
    data2.push_back(78);
    data2.push_back(57);
    data2.push_back(1 );
    data2.push_back(80);
    data2.push_back(92);
    data2.push_back(54);
    data2.push_back(40);
    data2.push_back(71);
    data2.push_back(56);
    data2.push_back(70);
    data2.push_back(16);
    data2.push_back(98);
    data2.push_back(1 );
    data2.push_back(21);
    data2.push_back(68);
    data2.push_back(34);
    data2.push_back(84);
    data2.push_back(37);
    data2.push_back(57);
    data2.push_back(4 );
    data2.push_back(96);
    data2.push_back(31);
    data2.push_back(89);
    data2.push_back(43);
    data2.push_back(14);
    data2.push_back(60);
    data2.push_back(21);
    data2.push_back(39);
    data2.push_back(54);
    data2.push_back(52);
    data2.push_back(85);
    data2.push_back(51);
    data2.push_back(9 );
    data2.push_back(63);
    data2.push_back(57);
    data2.push_back(65);
    data2.push_back(20);
    data2.push_back(21);
    data2.push_back(27);
    data2.push_back(27);
    data2.push_back(45);
    data2.push_back(27);
    data2.push_back(35);
    data2.push_back(18);
    data2.push_back(18);
    data2.push_back(26);
    data2.push_back(16);
    data2.push_back(11);
    data2.push_back(97);
    data2.push_back(71);
    data2.push_back(76);
    data2.push_back(45);
    data2.push_back(85);
    data2.push_back(61);
    data2.push_back(36);
    data2.push_back(47);
    data2.push_back(86);
    data2.push_back(21);
    data2.push_back(72);
    data2.push_back(10);
    data2.push_back(94);
    data2.push_back(12);
    data2.push_back(86);
    data2.push_back(0 );
    data2.push_back(14);
    data2.push_back(84);
    data2.push_back(47);
    data2.push_back(48);
    data2.push_back(31);
    data2.push_back(13);
    data2.push_back(31);
    data2.push_back(8 );
    data2.push_back(34);
    data2.push_back(88);
    data2.push_back(1 );
    data2.push_back(21);
    data2.push_back(94);
    data2.push_back(19);
    data2.push_back(74);
    data2.push_back(6 );
    data2.push_back(54);
    data2.push_back(24);
    data2.push_back(96);
    data2.push_back(72);

    CPPUNIT_ASSERT(abs(KolmogorovSmirnov::TwoSmpKSStat(data1, data2, 10)-0.05) < 0.001);
  }

  return;
}

void TestKolmogorovSmirnov::setUp()
{
}

void TestKolmogorovSmirnov::tearDown()
{
}

CppUnit::Test *TestKolmogorovSmirnov::suite()
{
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("TestKolmogorovSmirnov");

  suiteOfTests->addTest(new CppUnit::TestCaller<TestKolmogorovSmirnov>(
        "TestKolmogorovSmirnov", 
        &TestKolmogorovSmirnov::RunTest));

  return suiteOfTests;
}

















































































































































































































































































































































































































