// vim: set sw=2 ts=8:
/*
 * Copyright (c) 2017 Hardware-Software-CoDesign, University of
 * Erlangen-Nuremberg. All rights reserved.
 *
 *   This library is free software; you can redistribute it and/or modify it under
 *   the terms of the GNU Lesser General Public License as published by the Free
 *   Software Foundation; either version 2 of the License, or (at your option) any
 *   later version.
 *
 *   This library is distributed in the hope that it will be useful, but WITHOUT
 *   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 *   FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 *   details.
 *
 *   You should have received a copy of the GNU Lesser General Public License
 *   along with this library; if not, write to the Free Software Foundation, Inc.,
 *   59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
 *
 * --- This software and any associated documentation is provided "as is"
 *
 * IN NO EVENT SHALL HARDWARE-SOFTWARE-CODESIGN, UNIVERSITY OF ERLANGEN NUREMBERG
 * BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS
 * DOCUMENTATION, EVEN IF HARDWARE-SOFTWARE-CODESIGN, UNIVERSITY OF ERLANGEN
 * NUREMBERG HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * HARDWARE-SOFTWARE-CODESIGN, UNIVERSITY OF ERLANGEN NUREMBERG, SPECIFICALLY
 * DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE SOFTWARE PROVIDED
 * HEREUNDER IS ON AN "AS IS" BASIS, AND HARDWARE-SOFTWARE-CODESIGN, UNIVERSITY OF
 * ERLANGEN NUREMBERG HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
 * ENHANCEMENTS, OR MODIFICATIONS.
 */

#include <CoSupport/String/color.hpp>

#include <cassert>
#include <cstring>

#include <iostream>

namespace CoSupport { namespace String {

  struct ColorEntry {
    const char *colorName;
    const char  rgb[3];
  };

  static ColorEntry colorNames[] = {
      { "aliceblue", { '\xF0', '\xF8', '\xFF' } },
      { "antiquewhite", { '\xFA', '\xEB', '\xD7' } },
      { "antiquewhite1", { '\xFF', '\xEF', '\xDB' } },
      { "antiquewhite2", { '\xEE', '\xDF', '\xCC' } },
      { "antiquewhite3", { '\xCD', '\xC0', '\xB0' } },
      { "antiquewhite4", { '\x8B', '\x83', '\x78' } },
      { "aquamarine", { '\x7F', '\xFF', '\xD4' } },
      { "aquamarine1", { '\x7F', '\xFF', '\xD4' } },
      { "aquamarine2", { '\x76', '\xEE', '\xC6' } },
      { "aquamarine3", { '\x66', '\xCD', '\xAA' } },
      { "aquamarine4", { '\x45', '\x8B', '\x74' } },
      { "azure", { '\xF0', '\xFF', '\xFF' } },
      { "azure1", { '\xF0', '\xFF', '\xFF' } },
      { "azure2", { '\xE0', '\xEE', '\xEE' } },
      { "azure3", { '\xC1', '\xCD', '\xCD' } },
      { "azure4", { '\x83', '\x8B', '\x8B' } },
      { "beige", { '\xF5', '\xF5', '\xDC' } },
      { "bisque", { '\xFF', '\xE4', '\xC4' } },
      { "bisque1", { '\xFF', '\xE4', '\xC4' } },
      { "bisque2", { '\xEE', '\xD5', '\xB7' } },
      { "bisque3", { '\xCD', '\xB7', '\x9E' } },
      { "bisque4", { '\x8B', '\x7D', '\x6B' } },
      { "black", { '\x00', '\x00', '\x00' } },
      { "blanchedalmond", { '\xFF', '\xEB', '\xCD' } },
      { "blue", { '\x00', '\x00', '\xFF' } },
      { "blue1", { '\x00', '\x00', '\xFF' } },
      { "blue2", { '\x00', '\x00', '\xEE' } },
      { "blue3", { '\x00', '\x00', '\xCD' } },
      { "blue4", { '\x00', '\x00', '\x8B' } },
      { "blueviolet", { '\x8A', '\x2B', '\xE2' } },
      { "brown", { '\xA5', '\x2A', '\x2A' } },
      { "brown1", { '\xFF', '\x40', '\x40' } },
      { "brown2", { '\xEE', '\x3B', '\x3B' } },
      { "brown3", { '\xCD', '\x33', '\x33' } },
      { "brown4", { '\x8B', '\x23', '\x23' } },
      { "burlywood", { '\xDE', '\xB8', '\x87' } },
      { "burlywood1", { '\xFF', '\xD3', '\x9B' } },
      { "burlywood2", { '\xEE', '\xC5', '\x91' } },
      { "burlywood3", { '\xCD', '\xAA', '\x7D' } },
      { "burlywood4", { '\x8B', '\x73', '\x55' } },
      { "cadetblue", { '\x5F', '\x9E', '\xA0' } },
      { "cadetblue1", { '\x98', '\xF5', '\xFF' } },
      { "cadetblue2", { '\x8E', '\xE5', '\xEE' } },
      { "cadetblue3", { '\x7A', '\xC5', '\xCD' } },
      { "cadetblue4", { '\x53', '\x86', '\x8B' } },
      { "chartreuse", { '\x7F', '\xFF', '\x00' } },
      { "chartreuse1", { '\x7F', '\xFF', '\x00' } },
      { "chartreuse2", { '\x76', '\xEE', '\x00' } },
      { "chartreuse3", { '\x66', '\xCD', '\x00' } },
      { "chartreuse4", { '\x45', '\x8B', '\x00' } },
      { "chocolate", { '\xD2', '\x69', '\x1E' } },
      { "chocolate1", { '\xFF', '\x7F', '\x24' } },
      { "chocolate2", { '\xEE', '\x76', '\x21' } },
      { "chocolate3", { '\xCD', '\x66', '\x1D' } },
      { "chocolate4", { '\x8B', '\x45', '\x13' } },
      { "coral", { '\xFF', '\x7F', '\x50' } },
      { "coral1", { '\xFF', '\x72', '\x56' } },
      { "coral2", { '\xEE', '\x6A', '\x50' } },
      { "coral3", { '\xCD', '\x5B', '\x45' } },
      { "coral4", { '\x8B', '\x3E', '\x2F' } },
      { "cornflowerblue", { '\x64', '\x95', '\xED' } },
      { "cornsilk", { '\xFF', '\xF8', '\xDC' } },
      { "cornsilk1", { '\xFF', '\xF8', '\xDC' } },
      { "cornsilk2", { '\xEE', '\xE8', '\xCD' } },
      { "cornsilk3", { '\xCD', '\xC8', '\xB1' } },
      { "cornsilk4", { '\x8B', '\x88', '\x78' } },
      { "cyan", { '\x00', '\xFF', '\xFF' } },
      { "cyan1", { '\x00', '\xFF', '\xFF' } },
      { "cyan2", { '\x00', '\xEE', '\xEE' } },
      { "cyan3", { '\x00', '\xCD', '\xCD' } },
      { "cyan4", { '\x00', '\x8B', '\x8B' } },
      { "darkblue", { '\x00', '\x00', '\x8B' } },
      { "darkcyan", { '\x00', '\x8B', '\x8B' } },
      { "darkgoldenrod", { '\xB8', '\x86', '\x0B' } },
      { "darkgoldenrod1", { '\xFF', '\xB9', '\x0F' } },
      { "darkgoldenrod2", { '\xEE', '\xAD', '\x0E' } },
      { "darkgoldenrod3", { '\xCD', '\x95', '\x0C' } },
      { "darkgoldenrod4", { '\x8B', '\x65', '\x08' } },
      { "darkgray", { '\xA9', '\xA9', '\xA9' } },
      { "darkgreen", { '\x00', '\x64', '\x00' } },
      { "darkkhaki", { '\xBD', '\xB7', '\x6B' } },
      { "darkmagenta", { '\x8B', '\x00', '\x8B' } },
      { "darkolivegreen", { '\x55', '\x6B', '\x2F' } },
      { "darkolivegreen1", { '\xCA', '\xFF', '\x70' } },
      { "darkolivegreen2", { '\xBC', '\xEE', '\x68' } },
      { "darkolivegreen3", { '\xA2', '\xCD', '\x5A' } },
      { "darkolivegreen4", { '\x6E', '\x8B', '\x3D' } },
      { "darkorange", { '\xFF', '\x8C', '\x00' } },
      { "darkorange1", { '\xFF', '\x7F', '\x00' } },
      { "darkorange2", { '\xEE', '\x76', '\x00' } },
      { "darkorange3", { '\xCD', '\x66', '\x00' } },
      { "darkorange4", { '\x8B', '\x45', '\x00' } },
      { "darkorchid", { '\x99', '\x32', '\xCC' } },
      { "darkorchid1", { '\xBF', '\x3E', '\xFF' } },
      { "darkorchid2", { '\xB2', '\x3A', '\xEE' } },
      { "darkorchid3", { '\x9A', '\x32', '\xCD' } },
      { "darkorchid4", { '\x68', '\x22', '\x8B' } },
      { "darkred", { '\x8B', '\x00', '\x00' } },
      { "darksalmon", { '\xE9', '\x96', '\x7A' } },
      { "darkseagreen", { '\x8F', '\xBC', '\x8F' } },
      { "darkseagreen1", { '\xC1', '\xFF', '\xC1' } },
      { "darkseagreen2", { '\xB4', '\xEE', '\xB4' } },
      { "darkseagreen3", { '\x9B', '\xCD', '\x9B' } },
      { "darkseagreen4", { '\x69', '\x8B', '\x69' } },
      { "darkslateblue", { '\x48', '\x3D', '\x8B' } },
      { "darkslategray", { '\x2F', '\x4F', '\x4F' } },
      { "darkslategray1", { '\x97', '\xFF', '\xFF' } },
      { "darkslategray2", { '\x8D', '\xEE', '\xEE' } },
      { "darkslategray3", { '\x79', '\xCD', '\xCD' } },
      { "darkslategray4", { '\x52', '\x8B', '\x8B' } },
      { "darkturquoise", { '\x00', '\xCE', '\xD1' } },
      { "darkviolet", { '\x94', '\x00', '\xD3' } },
      { "deeppink", { '\xFF', '\x14', '\x93' } },
      { "deeppink1", { '\xFF', '\x14', '\x93' } },
      { "deeppink2", { '\xEE', '\x12', '\x89' } },
      { "deeppink3", { '\xCD', '\x10', '\x76' } },
      { "deeppink4", { '\x8B', '\x0A', '\x50' } },
      { "deepskyblue", { '\x00', '\xBF', '\xFF' } },
      { "deepskyblue1", { '\x00', '\xBF', '\xFF' } },
      { "deepskyblue2", { '\x00', '\xB2', '\xEE' } },
      { "deepskyblue3", { '\x00', '\x9A', '\xCD' } },
      { "deepskyblue4", { '\x00', '\x68', '\x8B' } },
      { "dimgray", { '\x69', '\x69', '\x69' } },
      { "dodgerblue", { '\x1E', '\x90', '\xFF' } },
      { "dodgerblue1", { '\x1E', '\x90', '\xFF' } },
      { "dodgerblue2", { '\x1C', '\x86', '\xEE' } },
      { "dodgerblue3", { '\x18', '\x74', '\xCD' } },
      { "dodgerblue4", { '\x10', '\x4E', '\x8B' } },
      { "firebrick", { '\xB2', '\x22', '\x22' } },
      { "firebrick1", { '\xFF', '\x30', '\x30' } },
      { "firebrick2", { '\xEE', '\x2C', '\x2C' } },
      { "firebrick3", { '\xCD', '\x26', '\x26' } },
      { "firebrick4", { '\x8B', '\x1A', '\x1A' } },
      { "floralwhite", { '\xFF', '\xFA', '\xF0' } },
      { "forestgreen", { '\x22', '\x8B', '\x22' } },
      { "gainsboro", { '\xDC', '\xDC', '\xDC' } },
      { "ghostwhite", { '\xF8', '\xF8', '\xFF' } },
      { "gold", { '\xFF', '\xD7', '\x00' } },
      { "gold1", { '\xFF', '\xD7', '\x00' } },
      { "gold2", { '\xEE', '\xC9', '\x00' } },
      { "gold3", { '\xCD', '\xAD', '\x00' } },
      { "gold4", { '\x8B', '\x75', '\x00' } },
      { "goldenrod", { '\xDA', '\xA5', '\x20' } },
      { "goldenrod1", { '\xFF', '\xC1', '\x25' } },
      { "goldenrod2", { '\xEE', '\xB4', '\x22' } },
      { "goldenrod3", { '\xCD', '\x9B', '\x1D' } },
      { "goldenrod4", { '\x8B', '\x69', '\x14' } },
      { "gray", { '\xBE', '\xBE', '\xBE' } },
      { "gray0", { '\x00', '\x00', '\x00' } },
      { "gray1", { '\x03', '\x03', '\x03' } },
      { "gray10", { '\x1A', '\x1A', '\x1A' } },
      { "gray100", { '\xFF', '\xFF', '\xFF' } },
      { "gray11", { '\x1C', '\x1C', '\x1C' } },
      { "gray12", { '\x1F', '\x1F', '\x1F' } },
      { "gray13", { '\x21', '\x21', '\x21' } },
      { "gray14", { '\x24', '\x24', '\x24' } },
      { "gray15", { '\x26', '\x26', '\x26' } },
      { "gray16", { '\x29', '\x29', '\x29' } },
      { "gray17", { '\x2B', '\x2B', '\x2B' } },
      { "gray18", { '\x2E', '\x2E', '\x2E' } },
      { "gray19", { '\x30', '\x30', '\x30' } },
      { "gray2", { '\x05', '\x05', '\x05' } },
      { "gray20", { '\x33', '\x33', '\x33' } },
      { "gray21", { '\x36', '\x36', '\x36' } },
      { "gray22", { '\x38', '\x38', '\x38' } },
      { "gray23", { '\x3B', '\x3B', '\x3B' } },
      { "gray24", { '\x3D', '\x3D', '\x3D' } },
      { "gray25", { '\x40', '\x40', '\x40' } },
      { "gray26", { '\x42', '\x42', '\x42' } },
      { "gray27", { '\x45', '\x45', '\x45' } },
      { "gray28", { '\x47', '\x47', '\x47' } },
      { "gray29", { '\x4A', '\x4A', '\x4A' } },
      { "gray3", { '\x08', '\x08', '\x08' } },
      { "gray30", { '\x4D', '\x4D', '\x4D' } },
      { "gray31", { '\x4F', '\x4F', '\x4F' } },
      { "gray32", { '\x52', '\x52', '\x52' } },
      { "gray33", { '\x54', '\x54', '\x54' } },
      { "gray34", { '\x57', '\x57', '\x57' } },
      { "gray35", { '\x59', '\x59', '\x59' } },
      { "gray36", { '\x5C', '\x5C', '\x5C' } },
      { "gray37", { '\x5E', '\x5E', '\x5E' } },
      { "gray38", { '\x61', '\x61', '\x61' } },
      { "gray39", { '\x63', '\x63', '\x63' } },
      { "gray4", { '\x0A', '\x0A', '\x0A' } },
      { "gray40", { '\x66', '\x66', '\x66' } },
      { "gray41", { '\x69', '\x69', '\x69' } },
      { "gray42", { '\x6B', '\x6B', '\x6B' } },
      { "gray43", { '\x6E', '\x6E', '\x6E' } },
      { "gray44", { '\x70', '\x70', '\x70' } },
      { "gray45", { '\x73', '\x73', '\x73' } },
      { "gray46", { '\x75', '\x75', '\x75' } },
      { "gray47", { '\x78', '\x78', '\x78' } },
      { "gray48", { '\x7A', '\x7A', '\x7A' } },
      { "gray49", { '\x7D', '\x7D', '\x7D' } },
      { "gray5", { '\x0D', '\x0D', '\x0D' } },
      { "gray50", { '\x7F', '\x7F', '\x7F' } },
      { "gray51", { '\x82', '\x82', '\x82' } },
      { "gray52", { '\x85', '\x85', '\x85' } },
      { "gray53", { '\x87', '\x87', '\x87' } },
      { "gray54", { '\x8A', '\x8A', '\x8A' } },
      { "gray55", { '\x8C', '\x8C', '\x8C' } },
      { "gray56", { '\x8F', '\x8F', '\x8F' } },
      { "gray57", { '\x91', '\x91', '\x91' } },
      { "gray58", { '\x94', '\x94', '\x94' } },
      { "gray59", { '\x96', '\x96', '\x96' } },
      { "gray6", { '\x0F', '\x0F', '\x0F' } },
      { "gray60", { '\x99', '\x99', '\x99' } },
      { "gray61", { '\x9C', '\x9C', '\x9C' } },
      { "gray62", { '\x9E', '\x9E', '\x9E' } },
      { "gray63", { '\xA1', '\xA1', '\xA1' } },
      { "gray64", { '\xA3', '\xA3', '\xA3' } },
      { "gray65", { '\xA6', '\xA6', '\xA6' } },
      { "gray66", { '\xA8', '\xA8', '\xA8' } },
      { "gray67", { '\xAB', '\xAB', '\xAB' } },
      { "gray68", { '\xAD', '\xAD', '\xAD' } },
      { "gray69", { '\xB0', '\xB0', '\xB0' } },
      { "gray7", { '\x12', '\x12', '\x12' } },
      { "gray70", { '\xB3', '\xB3', '\xB3' } },
      { "gray71", { '\xB5', '\xB5', '\xB5' } },
      { "gray72", { '\xB8', '\xB8', '\xB8' } },
      { "gray73", { '\xBA', '\xBA', '\xBA' } },
      { "gray74", { '\xBD', '\xBD', '\xBD' } },
      { "gray75", { '\xBF', '\xBF', '\xBF' } },
      { "gray76", { '\xC2', '\xC2', '\xC2' } },
      { "gray77", { '\xC4', '\xC4', '\xC4' } },
      { "gray78", { '\xC7', '\xC7', '\xC7' } },
      { "gray79", { '\xC9', '\xC9', '\xC9' } },
      { "gray8", { '\x14', '\x14', '\x14' } },
      { "gray80", { '\xCC', '\xCC', '\xCC' } },
      { "gray81", { '\xCF', '\xCF', '\xCF' } },
      { "gray82", { '\xD1', '\xD1', '\xD1' } },
      { "gray83", { '\xD4', '\xD4', '\xD4' } },
      { "gray84", { '\xD6', '\xD6', '\xD6' } },
      { "gray85", { '\xD9', '\xD9', '\xD9' } },
      { "gray86", { '\xDB', '\xDB', '\xDB' } },
      { "gray87", { '\xDE', '\xDE', '\xDE' } },
      { "gray88", { '\xE0', '\xE0', '\xE0' } },
      { "gray89", { '\xE3', '\xE3', '\xE3' } },
      { "gray9", { '\x17', '\x17', '\x17' } },
      { "gray90", { '\xE5', '\xE5', '\xE5' } },
      { "gray91", { '\xE8', '\xE8', '\xE8' } },
      { "gray92", { '\xEB', '\xEB', '\xEB' } },
      { "gray93", { '\xED', '\xED', '\xED' } },
      { "gray94", { '\xF0', '\xF0', '\xF0' } },
      { "gray95", { '\xF2', '\xF2', '\xF2' } },
      { "gray96", { '\xF5', '\xF5', '\xF5' } },
      { "gray97", { '\xF7', '\xF7', '\xF7' } },
      { "gray98", { '\xFA', '\xFA', '\xFA' } },
      { "gray99", { '\xFC', '\xFC', '\xFC' } },
      { "green", { '\x00', '\xFF', '\x00' } },
      { "green1", { '\x00', '\xFF', '\x00' } },
      { "green2", { '\x00', '\xEE', '\x00' } },
      { "green3", { '\x00', '\xCD', '\x00' } },
      { "green4", { '\x00', '\x8B', '\x00' } },
      { "greenyellow", { '\xAD', '\xFF', '\x2F' } },
      { "honeydew", { '\xF0', '\xFF', '\xF0' } },
      { "honeydew1", { '\xF0', '\xFF', '\xF0' } },
      { "honeydew2", { '\xE0', '\xEE', '\xE0' } },
      { "honeydew3", { '\xC1', '\xCD', '\xC1' } },
      { "honeydew4", { '\x83', '\x8B', '\x83' } },
      { "hotpink", { '\xFF', '\x69', '\xB4' } },
      { "hotpink1", { '\xFF', '\x6E', '\xB4' } },
      { "hotpink2", { '\xEE', '\x6A', '\xA7' } },
      { "hotpink3", { '\xCD', '\x60', '\x90' } },
      { "hotpink4", { '\x8B', '\x3A', '\x62' } },
      { "indianred", { '\xCD', '\x5C', '\x5C' } },
      { "indianred1", { '\xFF', '\x6A', '\x6A' } },
      { "indianred2", { '\xEE', '\x63', '\x63' } },
      { "indianred3", { '\xCD', '\x55', '\x55' } },
      { "indianred4", { '\x8B', '\x3A', '\x3A' } },
      { "ivory", { '\xFF', '\xFF', '\xF0' } },
      { "ivory1", { '\xFF', '\xFF', '\xF0' } },
      { "ivory2", { '\xEE', '\xEE', '\xE0' } },
      { "ivory3", { '\xCD', '\xCD', '\xC1' } },
      { "ivory4", { '\x8B', '\x8B', '\x83' } },
      { "khaki", { '\xF0', '\xE6', '\x8C' } },
      { "khaki1", { '\xFF', '\xF6', '\x8F' } },
      { "khaki2", { '\xEE', '\xE6', '\x85' } },
      { "khaki3", { '\xCD', '\xC6', '\x73' } },
      { "khaki4", { '\x8B', '\x86', '\x4E' } },
      { "lavender", { '\xE6', '\xE6', '\xFA' } },
      { "lavenderblush", { '\xFF', '\xF0', '\xF5' } },
      { "lavenderblush1", { '\xFF', '\xF0', '\xF5' } },
      { "lavenderblush2", { '\xEE', '\xE0', '\xE5' } },
      { "lavenderblush3", { '\xCD', '\xC1', '\xC5' } },
      { "lavenderblush4", { '\x8B', '\x83', '\x86' } },
      { "lawngreen", { '\x7C', '\xFC', '\x00' } },
      { "lemonchiffon", { '\xFF', '\xFA', '\xCD' } },
      { "lemonchiffon1", { '\xFF', '\xFA', '\xCD' } },
      { "lemonchiffon2", { '\xEE', '\xE9', '\xBF' } },
      { "lemonchiffon3", { '\xCD', '\xC9', '\xA5' } },
      { "lemonchiffon4", { '\x8B', '\x89', '\x70' } },
      { "lightblue", { '\xAD', '\xD8', '\xE6' } },
      { "lightblue1", { '\xBF', '\xEF', '\xFF' } },
      { "lightblue2", { '\xB2', '\xDF', '\xEE' } },
      { "lightblue3", { '\x9A', '\xC0', '\xCD' } },
      { "lightblue4", { '\x68', '\x83', '\x8B' } },
      { "lightcoral", { '\xF0', '\x80', '\x80' } },
      { "lightcyan", { '\xE0', '\xFF', '\xFF' } },
      { "lightcyan1", { '\xE0', '\xFF', '\xFF' } },
      { "lightcyan2", { '\xD1', '\xEE', '\xEE' } },
      { "lightcyan3", { '\xB4', '\xCD', '\xCD' } },
      { "lightcyan4", { '\x7A', '\x8B', '\x8B' } },
      { "lightgoldenrod", { '\xEE', '\xDD', '\x82' } },
      { "lightgoldenrod1", { '\xFF', '\xEC', '\x8B' } },
      { "lightgoldenrod2", { '\xEE', '\xDC', '\x82' } },
      { "lightgoldenrod3", { '\xCD', '\xBE', '\x70' } },
      { "lightgoldenrod4", { '\x8B', '\x81', '\x4C' } },
      { "lightgoldenrodyellow", { '\xFA', '\xFA', '\xD2' } },
      { "lightgray", { '\xD3', '\xD3', '\xD3' } },
      { "lightgreen", { '\x90', '\xEE', '\x90' } },
      { "lightgrey", { '\xD3', '\xD3', '\xD3' } },
      { "lightpink", { '\xFF', '\xB6', '\xC1' } },
      { "lightpink1", { '\xFF', '\xAE', '\xB9' } },
      { "lightpink2", { '\xEE', '\xA2', '\xAD' } },
      { "lightpink3", { '\xCD', '\x8C', '\x95' } },
      { "lightpink4", { '\x8B', '\x5F', '\x65' } },
      { "lightsalmon", { '\xFF', '\xA0', '\x7A' } },
      { "lightsalmon1", { '\xFF', '\xA0', '\x7A' } },
      { "lightsalmon2", { '\xEE', '\x95', '\x72' } },
      { "lightsalmon3", { '\xCD', '\x81', '\x62' } },
      { "lightsalmon4", { '\x8B', '\x57', '\x42' } },
      { "lightseagreen", { '\x20', '\xB2', '\xAA' } },
      { "lightskyblue", { '\x87', '\xCE', '\xFA' } },
      { "lightskyblue1", { '\xB0', '\xE2', '\xFF' } },
      { "lightskyblue2", { '\xA4', '\xD3', '\xEE' } },
      { "lightskyblue3", { '\x8D', '\xB6', '\xCD' } },
      { "lightskyblue4", { '\x60', '\x7B', '\x8B' } },
      { "lightslateblue", { '\x84', '\x70', '\xFF' } },
      { "lightslategray", { '\x77', '\x88', '\x99' } },
      { "lightsteelblue", { '\xB0', '\xC4', '\xDE' } },
      { "lightsteelblue1", { '\xCA', '\xE1', '\xFF' } },
      { "lightsteelblue2", { '\xBC', '\xD2', '\xEE' } },
      { "lightsteelblue3", { '\xA2', '\xB5', '\xCD' } },
      { "lightsteelblue4", { '\x6E', '\x7B', '\x8B' } },
      { "lightyellow", { '\xFF', '\xFF', '\xE0' } },
      { "lightyellow1", { '\xFF', '\xFF', '\xE0' } },
      { "lightyellow2", { '\xEE', '\xEE', '\xD1' } },
      { "lightyellow3", { '\xCD', '\xCD', '\xB4' } },
      { "lightyellow4", { '\x8B', '\x8B', '\x7A' } },
      { "limegreen", { '\x32', '\xCD', '\x32' } },
      { "linen", { '\xFA', '\xF0', '\xE6' } },
      { "magenta", { '\xFF', '\x00', '\xFF' } },
      { "magenta1", { '\xFF', '\x00', '\xFF' } },
      { "magenta2", { '\xEE', '\x00', '\xEE' } },
      { "magenta3", { '\xCD', '\x00', '\xCD' } },
      { "magenta4", { '\x8B', '\x00', '\x8B' } },
      { "maroon", { '\xB0', '\x30', '\x60' } },
      { "maroon1", { '\xFF', '\x34', '\xB3' } },
      { "maroon2", { '\xEE', '\x30', '\xA7' } },
      { "maroon3", { '\xCD', '\x29', '\x90' } },
      { "maroon4", { '\x8B', '\x1C', '\x62' } },
      { "mediumaquamarine", { '\x66', '\xCD', '\xAA' } },
      { "mediumblue", { '\x00', '\x00', '\xCD' } },
      { "mediumorchid", { '\xBA', '\x55', '\xD3' } },
      { "mediumorchid1", { '\xE0', '\x66', '\xFF' } },
      { "mediumorchid2", { '\xD1', '\x5F', '\xEE' } },
      { "mediumorchid3", { '\xB4', '\x52', '\xCD' } },
      { "mediumorchid4", { '\x7A', '\x37', '\x8B' } },
      { "mediumpurple", { '\x93', '\x70', '\xDB' } },
      { "mediumpurple1", { '\xAB', '\x82', '\xFF' } },
      { "mediumpurple2", { '\x9F', '\x79', '\xEE' } },
      { "mediumpurple3", { '\x89', '\x68', '\xCD' } },
      { "mediumpurple4", { '\x5D', '\x47', '\x8B' } },
      { "mediumseagreen", { '\x3C', '\xB3', '\x71' } },
      { "mediumslateblue", { '\x7B', '\x68', '\xEE' } },
      { "mediumspringgreen", { '\x00', '\xFA', '\x9A' } },
      { "mediumturquoise", { '\x48', '\xD1', '\xCC' } },
      { "mediumvioletred", { '\xC7', '\x15', '\x85' } },
      { "midnightblue", { '\x19', '\x19', '\x70' } },
      { "mintcream", { '\xF5', '\xFF', '\xFA' } },
      { "mistyrose", { '\xFF', '\xE4', '\xE1' } },
      { "mistyrose1", { '\xFF', '\xE4', '\xE1' } },
      { "mistyrose2", { '\xEE', '\xD5', '\xD2' } },
      { "mistyrose3", { '\xCD', '\xB7', '\xB5' } },
      { "mistyrose4", { '\x8B', '\x7D', '\x7B' } },
      { "moccasin", { '\xFF', '\xE4', '\xB5' } },
      { "navajowhite", { '\xFF', '\xDE', '\xAD' } },
      { "navajowhite1", { '\xFF', '\xDE', '\xAD' } },
      { "navajowhite2", { '\xEE', '\xCF', '\xA1' } },
      { "navajowhite3", { '\xCD', '\xB3', '\x8B' } },
      { "navajowhite4", { '\x8B', '\x79', '\x5E' } },
      { "navy", { '\x00', '\x00', '\x80' } },
      { "navyblue", { '\x00', '\x00', '\x80' } },
      { "oldlace", { '\xFD', '\xF5', '\xE6' } },
      { "olivedrab", { '\x6B', '\x8E', '\x23' } },
      { "olivedrab1", { '\xC0', '\xFF', '\x3E' } },
      { "olivedrab2", { '\xB3', '\xEE', '\x3A' } },
      { "olivedrab3", { '\x9A', '\xCD', '\x32' } },
      { "olivedrab4", { '\x69', '\x8B', '\x22' } },
      { "orange", { '\xFF', '\xA5', '\x00' } },
      { "orange1", { '\xFF', '\xA5', '\x00' } },
      { "orange2", { '\xEE', '\x9A', '\x00' } },
      { "orange3", { '\xCD', '\x85', '\x00' } },
      { "orange4", { '\x8B', '\x5A', '\x00' } },
      { "orangered", { '\xFF', '\x45', '\x00' } },
      { "orangered1", { '\xFF', '\x45', '\x00' } },
      { "orangered2", { '\xEE', '\x40', '\x00' } },
      { "orangered3", { '\xCD', '\x37', '\x00' } },
      { "orangered4", { '\x8B', '\x25', '\x00' } },
      { "orchid", { '\xDA', '\x70', '\xD6' } },
      { "orchid1", { '\xFF', '\x83', '\xFA' } },
      { "orchid2", { '\xEE', '\x7A', '\xE9' } },
      { "orchid3", { '\xCD', '\x69', '\xC9' } },
      { "orchid4", { '\x8B', '\x47', '\x89' } },
      { "palegoldenrod", { '\xEE', '\xE8', '\xAA' } },
      { "palegreen", { '\x98', '\xFB', '\x98' } },
      { "palegreen1", { '\x9A', '\xFF', '\x9A' } },
      { "palegreen2", { '\x90', '\xEE', '\x90' } },
      { "palegreen3", { '\x7C', '\xCD', '\x7C' } },
      { "palegreen4", { '\x54', '\x8B', '\x54' } },
      { "paleturquoise", { '\xAF', '\xEE', '\xEE' } },
      { "paleturquoise1", { '\xBB', '\xFF', '\xFF' } },
      { "paleturquoise2", { '\xAE', '\xEE', '\xEE' } },
      { "paleturquoise3", { '\x96', '\xCD', '\xCD' } },
      { "paleturquoise4", { '\x66', '\x8B', '\x8B' } },
      { "palevioletred", { '\xDB', '\x70', '\x93' } },
      { "palevioletred1", { '\xFF', '\x82', '\xAB' } },
      { "palevioletred2", { '\xEE', '\x79', '\x9F' } },
      { "palevioletred3", { '\xCD', '\x68', '\x89' } },
      { "palevioletred4", { '\x8B', '\x47', '\x5D' } },
      { "papayawhip", { '\xFF', '\xEF', '\xD5' } },
      { "peachpuff", { '\xFF', '\xDA', '\xB9' } },
      { "peachpuff1", { '\xFF', '\xDA', '\xB9' } },
      { "peachpuff2", { '\xEE', '\xCB', '\xAD' } },
      { "peachpuff3", { '\xCD', '\xAF', '\x95' } },
      { "peachpuff4", { '\x8B', '\x77', '\x65' } },
      { "peru", { '\xCD', '\x85', '\x3F' } },
      { "pink", { '\xFF', '\xC0', '\xCB' } },
      { "pink1", { '\xFF', '\xB5', '\xC5' } },
      { "pink2", { '\xEE', '\xA9', '\xB8' } },
      { "pink3", { '\xCD', '\x91', '\x9E' } },
      { "pink4", { '\x8B', '\x63', '\x6C' } },
      { "plum", { '\xDD', '\xA0', '\xDD' } },
      { "plum1", { '\xFF', '\xBB', '\xFF' } },
      { "plum2", { '\xEE', '\xAE', '\xEE' } },
      { "plum3", { '\xCD', '\x96', '\xCD' } },
      { "plum4", { '\x8B', '\x66', '\x8B' } },
      { "powderblue", { '\xB0', '\xE0', '\xE6' } },
      { "purple", { '\xA0', '\x20', '\xF0' } },
      { "purple1", { '\x9B', '\x30', '\xFF' } },
      { "purple2", { '\x91', '\x2C', '\xEE' } },
      { "purple3", { '\x7D', '\x26', '\xCD' } },
      { "purple4", { '\x55', '\x1A', '\x8B' } },
      { "red", { '\xFF', '\x00', '\x00' } },
      { "red1", { '\xFF', '\x00', '\x00' } },
      { "red2", { '\xEE', '\x00', '\x00' } },
      { "red3", { '\xCD', '\x00', '\x00' } },
      { "red4", { '\x8B', '\x00', '\x00' } },
      { "rosybrown", { '\xBC', '\x8F', '\x8F' } },
      { "rosybrown1", { '\xFF', '\xC1', '\xC1' } },
      { "rosybrown2", { '\xEE', '\xB4', '\xB4' } },
      { "rosybrown3", { '\xCD', '\x9B', '\x9B' } },
      { "rosybrown4", { '\x8B', '\x69', '\x69' } },
      { "royalblue", { '\x41', '\x69', '\xE1' } },
      { "royalblue1", { '\x48', '\x76', '\xFF' } },
      { "royalblue2", { '\x43', '\x6E', '\xEE' } },
      { "royalblue3", { '\x3A', '\x5F', '\xCD' } },
      { "royalblue4", { '\x27', '\x40', '\x8B' } },
      { "saddlebrown", { '\x8B', '\x45', '\x13' } },
      { "salmon", { '\xFA', '\x80', '\x72' } },
      { "salmon1", { '\xFF', '\x8C', '\x69' } },
      { "salmon2", { '\xEE', '\x82', '\x62' } },
      { "salmon3", { '\xCD', '\x70', '\x54' } },
      { "salmon4", { '\x8B', '\x4C', '\x39' } },
      { "sandybrown", { '\xF4', '\xA4', '\x60' } },
      { "seagreen", { '\x2E', '\x8B', '\x57' } },
      { "seagreen1", { '\x54', '\xFF', '\x9F' } },
      { "seagreen2", { '\x4E', '\xEE', '\x94' } },
      { "seagreen3", { '\x43', '\xCD', '\x80' } },
      { "seagreen4", { '\x2E', '\x8B', '\x57' } },
      { "seashell", { '\xFF', '\xF5', '\xEE' } },
      { "seashell1", { '\xFF', '\xF5', '\xEE' } },
      { "seashell2", { '\xEE', '\xE5', '\xDE' } },
      { "seashell3", { '\xCD', '\xC5', '\xBF' } },
      { "seashell4", { '\x8B', '\x86', '\x82' } },
      { "sienna", { '\xA0', '\x52', '\x2D' } },
      { "sienna1", { '\xFF', '\x82', '\x47' } },
      { "sienna2", { '\xEE', '\x79', '\x42' } },
      { "sienna3", { '\xCD', '\x68', '\x39' } },
      { "sienna4", { '\x8B', '\x47', '\x26' } },
      { "skyblue", { '\x87', '\xCE', '\xEB' } },
      { "skyblue1", { '\x87', '\xCE', '\xFF' } },
      { "skyblue2", { '\x7E', '\xC0', '\xEE' } },
      { "skyblue3", { '\x6C', '\xA6', '\xCD' } },
      { "skyblue4", { '\x4A', '\x70', '\x8B' } },
      { "slateblue", { '\x6A', '\x5A', '\xCD' } },
      { "slateblue1", { '\x83', '\x6F', '\xFF' } },
      { "slateblue2", { '\x7A', '\x67', '\xEE' } },
      { "slateblue3", { '\x69', '\x59', '\xCD' } },
      { "slateblue4", { '\x47', '\x3C', '\x8B' } },
      { "slategray", { '\x70', '\x80', '\x90' } },
      { "slategray1", { '\xC6', '\xE2', '\xFF' } },
      { "slategray2", { '\xB9', '\xD3', '\xEE' } },
      { "slategray3", { '\x9F', '\xB6', '\xCD' } },
      { "slategray4", { '\x6C', '\x7B', '\x8B' } },
      { "snow", { '\xFF', '\xFA', '\xFA' } },
      { "snow1", { '\xFF', '\xFA', '\xFA' } },
      { "snow2", { '\xEE', '\xE9', '\xE9' } },
      { "snow3", { '\xCD', '\xC9', '\xC9' } },
      { "snow4", { '\x8B', '\x89', '\x89' } },
      { "springgreen", { '\x00', '\xFF', '\x7F' } },
      { "springgreen1", { '\x00', '\xFF', '\x7F' } },
      { "springgreen2", { '\x00', '\xEE', '\x76' } },
      { "springgreen3", { '\x00', '\xCD', '\x66' } },
      { "springgreen4", { '\x00', '\x8B', '\x45' } },
      { "steelblue", { '\x46', '\x82', '\xB4' } },
      { "steelblue1", { '\x63', '\xB8', '\xFF' } },
      { "steelblue2", { '\x5C', '\xAC', '\xEE' } },
      { "steelblue3", { '\x4F', '\x94', '\xCD' } },
      { "steelblue4", { '\x36', '\x64', '\x8B' } },
      { "tan", { '\xD2', '\xB4', '\x8C' } },
      { "tan1", { '\xFF', '\xA5', '\x4F' } },
      { "tan2", { '\xEE', '\x9A', '\x49' } },
      { "tan3", { '\xCD', '\x85', '\x3F' } },
      { "tan4", { '\x8B', '\x5A', '\x2B' } },
      { "thistle", { '\xD8', '\xBF', '\xD8' } },
      { "thistle1", { '\xFF', '\xE1', '\xFF' } },
      { "thistle2", { '\xEE', '\xD2', '\xEE' } },
      { "thistle3", { '\xCD', '\xB5', '\xCD' } },
      { "thistle4", { '\x8B', '\x7B', '\x8B' } },
      { "tomato", { '\xFF', '\x63', '\x47' } },
      { "tomato1", { '\xFF', '\x63', '\x47' } },
      { "tomato2", { '\xEE', '\x5C', '\x42' } },
      { "tomato3", { '\xCD', '\x4F', '\x39' } },
      { "tomato4", { '\x8B', '\x36', '\x26' } },
      { "turquoise", { '\x40', '\xE0', '\xD0' } },
      { "turquoise1", { '\x00', '\xF5', '\xFF' } },
      { "turquoise2", { '\x00', '\xE5', '\xEE' } },
      { "turquoise3", { '\x00', '\xC5', '\xCD' } },
      { "turquoise4", { '\x00', '\x86', '\x8B' } },
      { "violet", { '\xEE', '\x82', '\xEE' } },
      { "violetred", { '\xD0', '\x20', '\x90' } },
      { "violetred1", { '\xFF', '\x3E', '\x96' } },
      { "violetred2", { '\xEE', '\x3A', '\x8C' } },
      { "violetred3", { '\xCD', '\x32', '\x78' } },
      { "violetred4", { '\x8B', '\x22', '\x52' } },
      { "wheat", { '\xF5', '\xDE', '\xB3' } },
      { "wheat1", { '\xFF', '\xE7', '\xBA' } },
      { "wheat2", { '\xEE', '\xD8', '\xAE' } },
      { "wheat3", { '\xCD', '\xBA', '\x96' } },
      { "wheat4", { '\x8B', '\x7E', '\x66' } },
      { "white", { '\xFF', '\xFF', '\xFF' } },
      { "whitesmoke", { '\xF5', '\xF5', '\xF5' } },
      { "yellow", { '\xFF', '\xFF', '\x00' } },
      { "yellow1", { '\xFF', '\xFF', '\x00' } },
      { "yellow2", { '\xEE', '\xEE', '\x00' } },
      { "yellow3", { '\xCD', '\xCD', '\x00' } },
      { "yellow4", { '\x8B', '\x8B', '\x00' } },
      { "yellowgreen", { '\x9A', '\xCD', '\x32' } },
    };

  COSUPPORT_STRING_API
  Color lookupColor(char const *const colorName) {
    unsigned int lower = 0;
    unsigned int upper = sizeof(colorNames)/sizeof(colorNames[0])-1;

#ifndef NDEBUG
    static bool checkSort = true;

    if (checkSort) {
      for (unsigned int i = 1; i <= upper; ++i)
        if (!(strcmp(colorNames[i-1].colorName, colorNames[i].colorName) < 0)) {
          std::cerr << colorNames[i-1].colorName << " < " << colorNames[i].colorName << std::endl;
          assert(strcmp(colorNames[i-1].colorName, colorNames[i].colorName) < 0);
        }
      checkSort = false;
    }
#endif //NDEBUG

    while (upper >= lower) {
      unsigned int mid = (lower+upper)/2;
      int cmp = strcmp(colorName, colorNames[mid].colorName);
      if (cmp > 0) {
        lower = std::max(mid, lower+1);
      } else if (cmp < 0) {
        upper = std::min(mid, upper-1);
      } else {
        return Color(
            static_cast<unsigned char>(colorNames[mid].rgb[0]),
            static_cast<unsigned char>(colorNames[mid].rgb[1]),
            static_cast<unsigned char>(colorNames[mid].rgb[2])
          );
      }
    }
    return Color({ 0u, 0u, 0u });
  }

  COSUPPORT_STRING_API
  Color lookupColor(std::string const &colorName)
    { return lookupColor(colorName.c_str()); }

  COSUPPORT_STRING_API
  Color getColor(unsigned int colorIndex) {
    static struct { const char rgb[3]; } colorEntries[] = {
        { '\xFF', '\xB1', '\xB1' },
        { '\xB1', '\xFF', '\xE3' },
        { '\xBA', '\xB1', '\xFF' },
        { '\xFF', '\xFA', '\xB1' },
        { '\xB1', '\xE1', '\xFF' },
        { '\xFF', '\x31', '\xFC' },
        { '\xC5', '\xFF', '\xB1' },
        { '\xFF', '\xD2', '\xB1' },
        { '\x9A', '\x7F', '\x7F' },
        { '\x7F', '\x82', '\x9A' },
        { '\x9A', '\x7F', '\x99' },
        { '\x7F', '\x9A', '\x98' },
        { '\x80', '\x9A', '\x7F' },
        { '\x9A', '\x98', '\x7F' },
    };
    if (colorIndex < sizeof(colorEntries)/sizeof(colorEntries[0])) {
      return Color(
          static_cast<unsigned char>(colorEntries[colorIndex].rgb[0]),
          static_cast<unsigned char>(colorEntries[colorIndex].rgb[1]),
          static_cast<unsigned char>(colorEntries[colorIndex].rgb[2])
        );
    } else {
      colorIndex -= sizeof(colorEntries)/sizeof(colorEntries[0]);
      colorIndex = colorIndex % (sizeof(colorNames)/sizeof(colorNames[0]));
      return Color(
          static_cast<unsigned char>(colorNames[colorIndex].rgb[0]),
          static_cast<unsigned char>(colorNames[colorIndex].rgb[1]),
          static_cast<unsigned char>(colorNames[colorIndex].rgb[2])
        );
    }
  }

} } // namespace CoSupport::String