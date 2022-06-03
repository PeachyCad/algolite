#include "pbPlots.hpp"
#include "supportLib.hpp"
#include "mingw.thread.h"
#include <vector>
#include <cmath>
#include <iostream>
#include <limits>
#include <string>
#include <sstream>
#define LLONG_MAX numeric_limits<long long>().max()

using namespace std;
#include "matrixOperations.cpp"

vector<double> xs;
vector<double> ys;
vector<double> xs1;
vector<double> ys1;
string complexity_class;
string complete_complexity_function;
const wchar_t* graph_title;
wstring w;

pair<double, double> linearRegression(vector<pair<long long, double> > func, bool log_first_var, bool log_second_var) {
	double error = 0;
	long long n = func.size();
	double a;
	double b;

	double v_sum = 0;
	double u_sum = 0;
	double vu_sum = 0;
	double vv_sum = 0;

	for (long long i = 0; i < n; i++) {

		double v = log_first_var ? log10(func[i].first) : func[i].first;
		double u = log_second_var ? log10(func[i].second) : func[i].second;

		v_sum += v;
		u_sum += u;
		vu_sum += v * u;
		vv_sum += v * v;
	}

	b = (n * vu_sum - v_sum * u_sum) / (n * vv_sum - v_sum * v_sum);
	a = (u_sum - b * v_sum) / n;

	for (long long i = 0; i < n; i++) {
		double v = log10(func[i].first);
		double u = log10(func[i].second);

		double estimated_u = a + b * v;
		error += (estimated_u - u) * (estimated_u - u);
	}

	error /= n;
	return make_pair(error, b);
}

Vector polynomialRegression(int polynomial_degree, vector<pair<long long, double> > func) {
	createXY(polynomial_degree, func);
	Matrix left_matrix(polynomial_degree + 1);
	Vector right_vector(polynomial_degree + 1);
	Vector result(polynomial_degree + 1);

	for (int i = 0; i < left_matrix.size(); i++) {
		Vector current_vector(polynomial_degree + 1);
		left_matrix[i] = current_vector;
	}

	multiplyMTM(x_matrix, left_matrix);
	multiplyMTV(x_matrix, y_vector, right_vector);

	getGaussianSolution(left_matrix, right_vector, result);

	return result;
}

string polynomialComplexityFunction(int polynomial_degree, vector<pair<long long, double> > func) {
	vector<double> resultCoefficients = polynomialRegression(polynomial_degree, func);
	stringstream result;

	result.precision(2);

	for (int i = polynomial_degree; i > 0; i--) {
		result << resultCoefficients[i] << " * x ^ " << i << " + ";
	}
	result << resultCoefficients[0];

	return result.str();
}

void deleteOverflowing(vector<pair<long long, double> >& func) {
	for (auto points_pair = func.begin(); points_pair != func.end(); points_pair++) {
		if ((*points_pair).second > 0.6 * LLONG_MAX || (*points_pair).second < 0.0) {
			func.erase(points_pair, func.end());
			break;
		}
	}
}

__thread long long _counter;
__thread long long par;
__thread int global_thread_number;
vector<pair<long long, double> > func_vec(96);

long long _factorial(long long n) {
	long long f = 1;
	_counter += 1;
	_counter += 1;
	for (long long i = 2; _counter += 1, i <= n; i++, _counter += 1) {f *= i, _counter += 2;    }
	_counter += 0;
	return f;
}

long long _r_fibonacci(int n) {
	if (_counter += 1, n <= 1) {_counter += 0;
		                    return n;    }
	_counter += 3;
	return _r_fibonacci(n - 1) + _r_fibonacci(n - 2);
}

void _binarySearch(int border, int key) {
	int x[ 1000 ];
	_counter += 0;
	x[ 0 ] = 0;
	x[ 1 ] = 1;
	x[ 2 ] = 2;
	x[ 3 ] = 3;
	x[ 4 ] = 4;
	x[ 5 ] = 5;
	x[ 6 ] = 6;
	x[ 7 ] = 7;
	x[ 8 ] = 8;
	x[ 9 ] = 9;
	x[ 10 ] = 10;
	x[ 11 ] = 11;
	x[ 12 ] = 12;
	x[ 13 ] = 13;
	x[ 14 ] = 14;
	x[ 15 ] = 15;
	x[ 16 ] = 16;
	x[ 17 ] = 17;
	x[ 18 ] = 18;
	x[ 19 ] = 19;
	x[ 20 ] = 20;
	x[ 21 ] = 21;
	x[ 22 ] = 22;
	x[ 23 ] = 23;
	x[ 24 ] = 24;
	x[ 25 ] = 25;
	x[ 26 ] = 26;
	x[ 27 ] = 27;
	x[ 28 ] = 28;
	x[ 29 ] = 29;
	x[ 30 ] = 30;
	x[ 31 ] = 31;
	x[ 32 ] = 32;
	x[ 33 ] = 33;
	x[ 34 ] = 34;
	x[ 35 ] = 35;
	x[ 36 ] = 36;
	x[ 37 ] = 37;
	x[ 38 ] = 38;
	x[ 39 ] = 39;
	x[ 40 ] = 40;
	x[ 41 ] = 41;
	x[ 42 ] = 42;
	x[ 43 ] = 43;
	x[ 44 ] = 44;
	x[ 45 ] = 45;
	x[ 46 ] = 46;
	x[ 47 ] = 47;
	x[ 48 ] = 48;
	x[ 49 ] = 49;
	x[ 50 ] = 50;
	x[ 51 ] = 51;
	x[ 52 ] = 52;
	x[ 53 ] = 53;
	x[ 54 ] = 54;
	x[ 55 ] = 55;
	x[ 56 ] = 56;
	x[ 57 ] = 57;
	x[ 58 ] = 58;
	x[ 59 ] = 59;
	x[ 60 ] = 60;
	x[ 61 ] = 61;
	x[ 62 ] = 62;
	x[ 63 ] = 63;
	x[ 64 ] = 64;
	x[ 65 ] = 65;
	x[ 66 ] = 66;
	x[ 67 ] = 67;
	x[ 68 ] = 68;
	x[ 69 ] = 69;
	x[ 70 ] = 70;
	x[ 71 ] = 71;
	x[ 72 ] = 72;
	x[ 73 ] = 73;
	x[ 74 ] = 74;
	x[ 75 ] = 75;
	x[ 76 ] = 76;
	x[ 77 ] = 77;
	x[ 78 ] = 78;
	x[ 79 ] = 79;
	x[ 80 ] = 80;
	x[ 81 ] = 81;
	x[ 82 ] = 82;
	x[ 83 ] = 83;
	x[ 84 ] = 84;
	x[ 85 ] = 85;
	x[ 86 ] = 86;
	x[ 87 ] = 87;
	x[ 88 ] = 88;
	x[ 89 ] = 89;
	x[ 90 ] = 90;
	x[ 91 ] = 91;
	x[ 92 ] = 92;
	x[ 93 ] = 93;
	x[ 94 ] = 94;
	x[ 95 ] = 95;
	x[ 96 ] = 96;
	x[ 97 ] = 97;
	x[ 98 ] = 98;
	x[ 99 ] = 99;
	x[ 100 ] = 100;
	x[ 101 ] = 101;
	x[ 102 ] = 102;
	x[ 103 ] = 103;
	x[ 104 ] = 104;
	x[ 105 ] = 105;
	x[ 106 ] = 106;
	x[ 107 ] = 107;
	x[ 108 ] = 108;
	x[ 109 ] = 109;
	x[ 110 ] = 110;
	x[ 111 ] = 111;
	x[ 112 ] = 112;
	x[ 113 ] = 113;
	x[ 114 ] = 114;
	x[ 115 ] = 115;
	x[ 116 ] = 116;
	x[ 117 ] = 117;
	x[ 118 ] = 118;
	x[ 119 ] = 119;
	x[ 120 ] = 120;
	x[ 121 ] = 121;
	x[ 122 ] = 122;
	x[ 123 ] = 123;
	x[ 124 ] = 124;
	x[ 125 ] = 125;
	x[ 126 ] = 126;
	x[ 127 ] = 127;
	x[ 128 ] = 128;
	x[ 129 ] = 129;
	x[ 130 ] = 130;
	x[ 131 ] = 131;
	x[ 132 ] = 132;
	x[ 133 ] = 133;
	x[ 134 ] = 134;
	x[ 135 ] = 135;
	x[ 136 ] = 136;
	x[ 137 ] = 137;
	x[ 138 ] = 138;
	x[ 139 ] = 139;
	x[ 140 ] = 140;
	x[ 141 ] = 141;
	x[ 142 ] = 142;
	x[ 143 ] = 143;
	x[ 144 ] = 144;
	x[ 145 ] = 145;
	x[ 146 ] = 146;
	x[ 147 ] = 147;
	x[ 148 ] = 148;
	x[ 149 ] = 149;
	x[ 150 ] = 150;
	x[ 151 ] = 151;
	x[ 152 ] = 152;
	x[ 153 ] = 153;
	x[ 154 ] = 154;
	x[ 155 ] = 155;
	x[ 156 ] = 156;
	x[ 157 ] = 157;
	x[ 158 ] = 158;
	x[ 159 ] = 159;
	x[ 160 ] = 160;
	x[ 161 ] = 161;
	x[ 162 ] = 162;
	x[ 163 ] = 163;
	x[ 164 ] = 164;
	x[ 165 ] = 165;
	x[ 166 ] = 166;
	x[ 167 ] = 167;
	x[ 168 ] = 168;
	x[ 169 ] = 169;
	x[ 170 ] = 170;
	x[ 171 ] = 171;
	x[ 172 ] = 172;
	x[ 173 ] = 173;
	x[ 174 ] = 174;
	x[ 175 ] = 175;
	x[ 176 ] = 176;
	x[ 177 ] = 177;
	x[ 178 ] = 178;
	x[ 179 ] = 179;
	x[ 180 ] = 180;
	x[ 181 ] = 181;
	x[ 182 ] = 182;
	x[ 183 ] = 183;
	x[ 184 ] = 184;
	x[ 185 ] = 185;
	x[ 186 ] = 186;
	x[ 187 ] = 187;
	x[ 188 ] = 188;
	x[ 189 ] = 189;
	x[ 190 ] = 190;
	x[ 191 ] = 191;
	x[ 192 ] = 192;
	x[ 193 ] = 193;
	x[ 194 ] = 194;
	x[ 195 ] = 195;
	x[ 196 ] = 196;
	x[ 197 ] = 197;
	x[ 198 ] = 198;
	x[ 199 ] = 199;
	x[ 200 ] = 200;
	x[ 201 ] = 201;
	x[ 202 ] = 202;
	x[ 203 ] = 203;
	x[ 204 ] = 204;
	x[ 205 ] = 205;
	x[ 206 ] = 206;
	x[ 207 ] = 207;
	x[ 208 ] = 208;
	x[ 209 ] = 209;
	x[ 210 ] = 210;
	x[ 211 ] = 211;
	x[ 212 ] = 212;
	x[ 213 ] = 213;
	x[ 214 ] = 214;
	x[ 215 ] = 215;
	x[ 216 ] = 216;
	x[ 217 ] = 217;
	x[ 218 ] = 218;
	x[ 219 ] = 219;
	x[ 220 ] = 220;
	x[ 221 ] = 221;
	x[ 222 ] = 222;
	x[ 223 ] = 223;
	x[ 224 ] = 224;
	x[ 225 ] = 225;
	x[ 226 ] = 226;
	x[ 227 ] = 227;
	x[ 228 ] = 228;
	x[ 229 ] = 229;
	x[ 230 ] = 230;
	x[ 231 ] = 231;
	x[ 232 ] = 232;
	x[ 233 ] = 233;
	x[ 234 ] = 234;
	x[ 235 ] = 235;
	x[ 236 ] = 236;
	x[ 237 ] = 237;
	x[ 238 ] = 238;
	x[ 239 ] = 239;
	x[ 240 ] = 240;
	x[ 241 ] = 241;
	x[ 242 ] = 242;
	x[ 243 ] = 243;
	x[ 244 ] = 244;
	x[ 245 ] = 245;
	x[ 246 ] = 246;
	x[ 247 ] = 247;
	x[ 248 ] = 248;
	x[ 249 ] = 249;
	x[ 250 ] = 250;
	x[ 251 ] = 251;
	x[ 252 ] = 252;
	x[ 253 ] = 253;
	x[ 254 ] = 254;
	x[ 255 ] = 255;
	x[ 256 ] = 256;
	x[ 257 ] = 257;
	x[ 258 ] = 258;
	x[ 259 ] = 259;
	x[ 260 ] = 260;
	x[ 261 ] = 261;
	x[ 262 ] = 262;
	x[ 263 ] = 263;
	x[ 264 ] = 264;
	x[ 265 ] = 265;
	x[ 266 ] = 266;
	x[ 267 ] = 267;
	x[ 268 ] = 268;
	x[ 269 ] = 269;
	x[ 270 ] = 270;
	x[ 271 ] = 271;
	x[ 272 ] = 272;
	x[ 273 ] = 273;
	x[ 274 ] = 274;
	x[ 275 ] = 275;
	x[ 276 ] = 276;
	x[ 277 ] = 277;
	x[ 278 ] = 278;
	x[ 279 ] = 279;
	x[ 280 ] = 280;
	x[ 281 ] = 281;
	x[ 282 ] = 282;
	x[ 283 ] = 283;
	x[ 284 ] = 284;
	x[ 285 ] = 285;
	x[ 286 ] = 286;
	x[ 287 ] = 287;
	x[ 288 ] = 288;
	x[ 289 ] = 289;
	x[ 290 ] = 290;
	x[ 291 ] = 291;
	x[ 292 ] = 292;
	x[ 293 ] = 293;
	x[ 294 ] = 294;
	x[ 295 ] = 295;
	x[ 296 ] = 296;
	x[ 297 ] = 297;
	x[ 298 ] = 298;
	x[ 299 ] = 299;
	x[ 300 ] = 300;
	x[ 301 ] = 301;
	x[ 302 ] = 302;
	x[ 303 ] = 303;
	x[ 304 ] = 304;
	x[ 305 ] = 305;
	x[ 306 ] = 306;
	x[ 307 ] = 307;
	x[ 308 ] = 308;
	x[ 309 ] = 309;
	x[ 310 ] = 310;
	x[ 311 ] = 311;
	x[ 312 ] = 312;
	x[ 313 ] = 313;
	x[ 314 ] = 314;
	x[ 315 ] = 315;
	x[ 316 ] = 316;
	x[ 317 ] = 317;
	x[ 318 ] = 318;
	x[ 319 ] = 319;
	x[ 320 ] = 320;
	x[ 321 ] = 321;
	x[ 322 ] = 322;
	x[ 323 ] = 323;
	x[ 324 ] = 324;
	x[ 325 ] = 325;
	x[ 326 ] = 326;
	x[ 327 ] = 327;
	x[ 328 ] = 328;
	x[ 329 ] = 329;
	x[ 330 ] = 330;
	x[ 331 ] = 331;
	x[ 332 ] = 332;
	x[ 333 ] = 333;
	x[ 334 ] = 334;
	x[ 335 ] = 335;
	x[ 336 ] = 336;
	x[ 337 ] = 337;
	x[ 338 ] = 338;
	x[ 339 ] = 339;
	x[ 340 ] = 340;
	x[ 341 ] = 341;
	x[ 342 ] = 342;
	x[ 343 ] = 343;
	x[ 344 ] = 344;
	x[ 345 ] = 345;
	x[ 346 ] = 346;
	x[ 347 ] = 347;
	x[ 348 ] = 348;
	x[ 349 ] = 349;
	x[ 350 ] = 350;
	x[ 351 ] = 351;
	x[ 352 ] = 352;
	x[ 353 ] = 353;
	x[ 354 ] = 354;
	x[ 355 ] = 355;
	x[ 356 ] = 356;
	x[ 357 ] = 357;
	x[ 358 ] = 358;
	x[ 359 ] = 359;
	x[ 360 ] = 360;
	x[ 361 ] = 361;
	x[ 362 ] = 362;
	x[ 363 ] = 363;
	x[ 364 ] = 364;
	x[ 365 ] = 365;
	x[ 366 ] = 366;
	x[ 367 ] = 367;
	x[ 368 ] = 368;
	x[ 369 ] = 369;
	x[ 370 ] = 370;
	x[ 371 ] = 371;
	x[ 372 ] = 372;
	x[ 373 ] = 373;
	x[ 374 ] = 374;
	x[ 375 ] = 375;
	x[ 376 ] = 376;
	x[ 377 ] = 377;
	x[ 378 ] = 378;
	x[ 379 ] = 379;
	x[ 380 ] = 380;
	x[ 381 ] = 381;
	x[ 382 ] = 382;
	x[ 383 ] = 383;
	x[ 384 ] = 384;
	x[ 385 ] = 385;
	x[ 386 ] = 386;
	x[ 387 ] = 387;
	x[ 388 ] = 388;
	x[ 389 ] = 389;
	x[ 390 ] = 390;
	x[ 391 ] = 391;
	x[ 392 ] = 392;
	x[ 393 ] = 393;
	x[ 394 ] = 394;
	x[ 395 ] = 395;
	x[ 396 ] = 396;
	x[ 397 ] = 397;
	x[ 398 ] = 398;
	x[ 399 ] = 399;
	x[ 400 ] = 400;
	x[ 401 ] = 401;
	x[ 402 ] = 402;
	x[ 403 ] = 403;
	x[ 404 ] = 404;
	x[ 405 ] = 405;
	x[ 406 ] = 406;
	x[ 407 ] = 407;
	x[ 408 ] = 408;
	x[ 409 ] = 409;
	x[ 410 ] = 410;
	x[ 411 ] = 411;
	x[ 412 ] = 412;
	x[ 413 ] = 413;
	x[ 414 ] = 414;
	x[ 415 ] = 415;
	x[ 416 ] = 416;
	x[ 417 ] = 417;
	x[ 418 ] = 418;
	x[ 419 ] = 419;
	x[ 420 ] = 420;
	x[ 421 ] = 421;
	x[ 422 ] = 422;
	x[ 423 ] = 423;
	x[ 424 ] = 424;
	x[ 425 ] = 425;
	x[ 426 ] = 426;
	x[ 427 ] = 427;
	x[ 428 ] = 428;
	x[ 429 ] = 429;
	x[ 430 ] = 430;
	x[ 431 ] = 431;
	x[ 432 ] = 432;
	x[ 433 ] = 433;
	x[ 434 ] = 434;
	x[ 435 ] = 435;
	x[ 436 ] = 436;
	x[ 437 ] = 437;
	x[ 438 ] = 438;
	x[ 439 ] = 439;
	x[ 440 ] = 440;
	x[ 441 ] = 441;
	x[ 442 ] = 442;
	x[ 443 ] = 443;
	x[ 444 ] = 444;
	x[ 445 ] = 445;
	x[ 446 ] = 446;
	x[ 447 ] = 447;
	x[ 448 ] = 448;
	x[ 449 ] = 449;
	x[ 450 ] = 450;
	x[ 451 ] = 451;
	x[ 452 ] = 452;
	x[ 453 ] = 453;
	x[ 454 ] = 454;
	x[ 455 ] = 455;
	x[ 456 ] = 456;
	x[ 457 ] = 457;
	x[ 458 ] = 458;
	x[ 459 ] = 459;
	x[ 460 ] = 460;
	x[ 461 ] = 461;
	x[ 462 ] = 462;
	x[ 463 ] = 463;
	x[ 464 ] = 464;
	x[ 465 ] = 465;
	x[ 466 ] = 466;
	x[ 467 ] = 467;
	x[ 468 ] = 468;
	x[ 469 ] = 469;
	x[ 470 ] = 470;
	x[ 471 ] = 471;
	x[ 472 ] = 472;
	x[ 473 ] = 473;
	x[ 474 ] = 474;
	x[ 475 ] = 475;
	x[ 476 ] = 476;
	x[ 477 ] = 477;
	x[ 478 ] = 478;
	x[ 479 ] = 479;
	x[ 480 ] = 480;
	x[ 481 ] = 481;
	x[ 482 ] = 482;
	x[ 483 ] = 483;
	x[ 484 ] = 484;
	x[ 485 ] = 485;
	x[ 486 ] = 486;
	x[ 487 ] = 487;
	x[ 488 ] = 488;
	x[ 489 ] = 489;
	x[ 490 ] = 490;
	x[ 491 ] = 491;
	x[ 492 ] = 492;
	x[ 493 ] = 493;
	x[ 494 ] = 494;
	x[ 495 ] = 495;
	x[ 496 ] = 496;
	x[ 497 ] = 497;
	x[ 498 ] = 498;
	x[ 499 ] = 499;
	x[ 500 ] = 500;
	x[ 501 ] = 501;
	x[ 502 ] = 502;
	x[ 503 ] = 503;
	x[ 504 ] = 504;
	x[ 505 ] = 505;
	x[ 506 ] = 506;
	x[ 507 ] = 507;
	x[ 508 ] = 508;
	x[ 509 ] = 509;
	x[ 510 ] = 510;
	x[ 511 ] = 511;
	x[ 512 ] = 512;
	x[ 513 ] = 513;
	x[ 514 ] = 514;
	x[ 515 ] = 515;
	x[ 516 ] = 516;
	x[ 517 ] = 517;
	x[ 518 ] = 518;
	x[ 519 ] = 519;
	x[ 520 ] = 520;
	x[ 521 ] = 521;
	x[ 522 ] = 522;
	x[ 523 ] = 523;
	x[ 524 ] = 524;
	x[ 525 ] = 525;
	x[ 526 ] = 526;
	x[ 527 ] = 527;
	x[ 528 ] = 528;
	x[ 529 ] = 529;
	x[ 530 ] = 530;
	x[ 531 ] = 531;
	x[ 532 ] = 532;
	x[ 533 ] = 533;
	x[ 534 ] = 534;
	x[ 535 ] = 535;
	x[ 536 ] = 536;
	x[ 537 ] = 537;
	x[ 538 ] = 538;
	x[ 539 ] = 539;
	x[ 540 ] = 540;
	x[ 541 ] = 541;
	x[ 542 ] = 542;
	x[ 543 ] = 543;
	x[ 544 ] = 544;
	x[ 545 ] = 545;
	x[ 546 ] = 546;
	x[ 547 ] = 547;
	x[ 548 ] = 548;
	x[ 549 ] = 549;
	x[ 550 ] = 550;
	x[ 551 ] = 551;
	x[ 552 ] = 552;
	x[ 553 ] = 553;
	x[ 554 ] = 554;
	x[ 555 ] = 555;
	x[ 556 ] = 556;
	x[ 557 ] = 557;
	x[ 558 ] = 558;
	x[ 559 ] = 559;
	x[ 560 ] = 560;
	x[ 561 ] = 561;
	x[ 562 ] = 562;
	x[ 563 ] = 563;
	x[ 564 ] = 564;
	x[ 565 ] = 565;
	x[ 566 ] = 566;
	x[ 567 ] = 567;
	x[ 568 ] = 568;
	x[ 569 ] = 569;
	x[ 570 ] = 570;
	x[ 571 ] = 571;
	x[ 572 ] = 572;
	x[ 573 ] = 573;
	x[ 574 ] = 574;
	x[ 575 ] = 575;
	x[ 576 ] = 576;
	x[ 577 ] = 577;
	x[ 578 ] = 578;
	x[ 579 ] = 579;
	x[ 580 ] = 580;
	x[ 581 ] = 581;
	x[ 582 ] = 582;
	x[ 583 ] = 583;
	x[ 584 ] = 584;
	x[ 585 ] = 585;
	x[ 586 ] = 586;
	x[ 587 ] = 587;
	x[ 588 ] = 588;
	x[ 589 ] = 589;
	x[ 590 ] = 590;
	x[ 591 ] = 591;
	x[ 592 ] = 592;
	x[ 593 ] = 593;
	x[ 594 ] = 594;
	x[ 595 ] = 595;
	x[ 596 ] = 596;
	x[ 597 ] = 597;
	x[ 598 ] = 598;
	x[ 599 ] = 599;
	x[ 600 ] = 600;
	x[ 601 ] = 601;
	x[ 602 ] = 602;
	x[ 603 ] = 603;
	x[ 604 ] = 604;
	x[ 605 ] = 605;
	x[ 606 ] = 606;
	x[ 607 ] = 607;
	x[ 608 ] = 608;
	x[ 609 ] = 609;
	x[ 610 ] = 610;
	x[ 611 ] = 611;
	x[ 612 ] = 612;
	x[ 613 ] = 613;
	x[ 614 ] = 614;
	x[ 615 ] = 615;
	x[ 616 ] = 616;
	x[ 617 ] = 617;
	x[ 618 ] = 618;
	x[ 619 ] = 619;
	x[ 620 ] = 620;
	x[ 621 ] = 621;
	x[ 622 ] = 622;
	x[ 623 ] = 623;
	x[ 624 ] = 624;
	x[ 625 ] = 625;
	x[ 626 ] = 626;
	x[ 627 ] = 627;
	x[ 628 ] = 628;
	x[ 629 ] = 629;
	x[ 630 ] = 630;
	x[ 631 ] = 631;
	x[ 632 ] = 632;
	x[ 633 ] = 633;
	x[ 634 ] = 634;
	x[ 635 ] = 635;
	x[ 636 ] = 636;
	x[ 637 ] = 637;
	x[ 638 ] = 638;
	x[ 639 ] = 639;
	x[ 640 ] = 640;
	x[ 641 ] = 641;
	x[ 642 ] = 642;
	x[ 643 ] = 643;
	x[ 644 ] = 644;
	x[ 645 ] = 645;
	x[ 646 ] = 646;
	x[ 647 ] = 647;
	x[ 648 ] = 648;
	x[ 649 ] = 649;
	x[ 650 ] = 650;
	x[ 651 ] = 651;
	x[ 652 ] = 652;
	x[ 653 ] = 653;
	x[ 654 ] = 654;
	x[ 655 ] = 655;
	x[ 656 ] = 656;
	x[ 657 ] = 657;
	x[ 658 ] = 658;
	x[ 659 ] = 659;
	x[ 660 ] = 660;
	x[ 661 ] = 661;
	x[ 662 ] = 662;
	x[ 663 ] = 663;
	x[ 664 ] = 664;
	x[ 665 ] = 665;
	x[ 666 ] = 666;
	x[ 667 ] = 667;
	x[ 668 ] = 668;
	x[ 669 ] = 669;
	x[ 670 ] = 670;
	x[ 671 ] = 671;
	x[ 672 ] = 672;
	x[ 673 ] = 673;
	x[ 674 ] = 674;
	x[ 675 ] = 675;
	x[ 676 ] = 676;
	x[ 677 ] = 677;
	x[ 678 ] = 678;
	x[ 679 ] = 679;
	x[ 680 ] = 680;
	x[ 681 ] = 681;
	x[ 682 ] = 682;
	x[ 683 ] = 683;
	x[ 684 ] = 684;
	x[ 685 ] = 685;
	x[ 686 ] = 686;
	x[ 687 ] = 687;
	x[ 688 ] = 688;
	x[ 689 ] = 689;
	x[ 690 ] = 690;
	x[ 691 ] = 691;
	x[ 692 ] = 692;
	x[ 693 ] = 693;
	x[ 694 ] = 694;
	x[ 695 ] = 695;
	x[ 696 ] = 696;
	x[ 697 ] = 697;
	x[ 698 ] = 698;
	x[ 699 ] = 699;
	x[ 700 ] = 700;
	x[ 701 ] = 701;
	x[ 702 ] = 702;
	x[ 703 ] = 703;
	x[ 704 ] = 704;
	x[ 705 ] = 705;
	x[ 706 ] = 706;
	x[ 707 ] = 707;
	x[ 708 ] = 708;
	x[ 709 ] = 709;
	x[ 710 ] = 710;
	x[ 711 ] = 711;
	x[ 712 ] = 712;
	x[ 713 ] = 713;
	x[ 714 ] = 714;
	x[ 715 ] = 715;
	x[ 716 ] = 716;
	x[ 717 ] = 717;
	x[ 718 ] = 718;
	x[ 719 ] = 719;
	x[ 720 ] = 720;
	x[ 721 ] = 721;
	x[ 722 ] = 722;
	x[ 723 ] = 723;
	x[ 724 ] = 724;
	x[ 725 ] = 725;
	x[ 726 ] = 726;
	x[ 727 ] = 727;
	x[ 728 ] = 728;
	x[ 729 ] = 729;
	x[ 730 ] = 730;
	x[ 731 ] = 731;
	x[ 732 ] = 732;
	x[ 733 ] = 733;
	x[ 734 ] = 734;
	x[ 735 ] = 735;
	x[ 736 ] = 736;
	x[ 737 ] = 737;
	x[ 738 ] = 738;
	x[ 739 ] = 739;
	x[ 740 ] = 740;
	x[ 741 ] = 741;
	x[ 742 ] = 742;
	x[ 743 ] = 743;
	x[ 744 ] = 744;
	x[ 745 ] = 745;
	x[ 746 ] = 746;
	x[ 747 ] = 747;
	x[ 748 ] = 748;
	x[ 749 ] = 749;
	x[ 750 ] = 750;
	x[ 751 ] = 751;
	x[ 752 ] = 752;
	x[ 753 ] = 753;
	x[ 754 ] = 754;
	x[ 755 ] = 755;
	x[ 756 ] = 756;
	x[ 757 ] = 757;
	x[ 758 ] = 758;
	x[ 759 ] = 759;
	x[ 760 ] = 760;
	x[ 761 ] = 761;
	x[ 762 ] = 762;
	x[ 763 ] = 763;
	x[ 764 ] = 764;
	x[ 765 ] = 765;
	x[ 766 ] = 766;
	x[ 767 ] = 767;
	x[ 768 ] = 768;
	x[ 769 ] = 769;
	x[ 770 ] = 770;
	x[ 771 ] = 771;
	x[ 772 ] = 772;
	x[ 773 ] = 773;
	x[ 774 ] = 774;
	x[ 775 ] = 775;
	x[ 776 ] = 776;
	x[ 777 ] = 777;
	x[ 778 ] = 778;
	x[ 779 ] = 779;
	x[ 780 ] = 780;
	x[ 781 ] = 781;
	x[ 782 ] = 782;
	x[ 783 ] = 783;
	x[ 784 ] = 784;
	x[ 785 ] = 785;
	x[ 786 ] = 786;
	x[ 787 ] = 787;
	x[ 788 ] = 788;
	x[ 789 ] = 789;
	x[ 790 ] = 790;
	x[ 791 ] = 791;
	x[ 792 ] = 792;
	x[ 793 ] = 793;
	x[ 794 ] = 794;
	x[ 795 ] = 795;
	x[ 796 ] = 796;
	x[ 797 ] = 797;
	x[ 798 ] = 798;
	x[ 799 ] = 799;
	x[ 800 ] = 800;
	x[ 801 ] = 801;
	x[ 802 ] = 802;
	x[ 803 ] = 803;
	x[ 804 ] = 804;
	x[ 805 ] = 805;
	x[ 806 ] = 806;
	x[ 807 ] = 807;
	x[ 808 ] = 808;
	x[ 809 ] = 809;
	x[ 810 ] = 810;
	x[ 811 ] = 811;
	x[ 812 ] = 812;
	x[ 813 ] = 813;
	x[ 814 ] = 814;
	x[ 815 ] = 815;
	x[ 816 ] = 816;
	x[ 817 ] = 817;
	x[ 818 ] = 818;
	x[ 819 ] = 819;
	x[ 820 ] = 820;
	x[ 821 ] = 821;
	x[ 822 ] = 822;
	x[ 823 ] = 823;
	x[ 824 ] = 824;
	x[ 825 ] = 825;
	x[ 826 ] = 826;
	x[ 827 ] = 827;
	x[ 828 ] = 828;
	x[ 829 ] = 829;
	x[ 830 ] = 830;
	x[ 831 ] = 831;
	x[ 832 ] = 832;
	x[ 833 ] = 833;
	x[ 834 ] = 834;
	x[ 835 ] = 835;
	x[ 836 ] = 836;
	x[ 837 ] = 837;
	x[ 838 ] = 838;
	x[ 839 ] = 839;
	x[ 840 ] = 840;
	x[ 841 ] = 841;
	x[ 842 ] = 842;
	x[ 843 ] = 843;
	x[ 844 ] = 844;
	x[ 845 ] = 845;
	x[ 846 ] = 846;
	x[ 847 ] = 847;
	x[ 848 ] = 848;
	x[ 849 ] = 849;
	x[ 850 ] = 850;
	x[ 851 ] = 851;
	x[ 852 ] = 852;
	x[ 853 ] = 853;
	x[ 854 ] = 854;
	x[ 855 ] = 855;
	x[ 856 ] = 856;
	x[ 857 ] = 857;
	x[ 858 ] = 858;
	x[ 859 ] = 859;
	x[ 860 ] = 860;
	x[ 861 ] = 861;
	x[ 862 ] = 862;
	x[ 863 ] = 863;
	x[ 864 ] = 864;
	x[ 865 ] = 865;
	x[ 866 ] = 866;
	x[ 867 ] = 867;
	x[ 868 ] = 868;
	x[ 869 ] = 869;
	x[ 870 ] = 870;
	x[ 871 ] = 871;
	x[ 872 ] = 872;
	x[ 873 ] = 873;
	x[ 874 ] = 874;
	x[ 875 ] = 875;
	x[ 876 ] = 876;
	x[ 877 ] = 877;
	x[ 878 ] = 878;
	x[ 879 ] = 879;
	x[ 880 ] = 880;
	x[ 881 ] = 881;
	x[ 882 ] = 882;
	x[ 883 ] = 883;
	x[ 884 ] = 884;
	x[ 885 ] = 885;
	x[ 886 ] = 886;
	x[ 887 ] = 887;
	x[ 888 ] = 888;
	x[ 889 ] = 889;
	x[ 890 ] = 890;
	x[ 891 ] = 891;
	x[ 892 ] = 892;
	x[ 893 ] = 893;
	x[ 894 ] = 894;
	x[ 895 ] = 895;
	x[ 896 ] = 896;
	x[ 897 ] = 897;
	x[ 898 ] = 898;
	x[ 899 ] = 899;
	x[ 900 ] = 900;
	x[ 901 ] = 901;
	x[ 902 ] = 902;
	x[ 903 ] = 903;
	x[ 904 ] = 904;
	x[ 905 ] = 905;
	x[ 906 ] = 906;
	x[ 907 ] = 907;
	x[ 908 ] = 908;
	x[ 909 ] = 909;
	x[ 910 ] = 910;
	x[ 911 ] = 911;
	x[ 912 ] = 912;
	x[ 913 ] = 913;
	x[ 914 ] = 914;
	x[ 915 ] = 915;
	x[ 916 ] = 916;
	x[ 917 ] = 917;
	x[ 918 ] = 918;
	x[ 919 ] = 919;
	x[ 920 ] = 920;
	x[ 921 ] = 921;
	x[ 922 ] = 922;
	x[ 923 ] = 923;
	x[ 924 ] = 924;
	x[ 925 ] = 925;
	x[ 926 ] = 926;
	x[ 927 ] = 927;
	x[ 928 ] = 928;
	x[ 929 ] = 929;
	x[ 930 ] = 930;
	x[ 931 ] = 931;
	x[ 932 ] = 932;
	x[ 933 ] = 933;
	x[ 934 ] = 934;
	x[ 935 ] = 935;
	x[ 936 ] = 936;
	x[ 937 ] = 937;
	x[ 938 ] = 938;
	x[ 939 ] = 939;
	x[ 940 ] = 940;
	x[ 941 ] = 941;
	x[ 942 ] = 942;
	x[ 943 ] = 943;
	x[ 944 ] = 944;
	x[ 945 ] = 945;
	x[ 946 ] = 946;
	x[ 947 ] = 947;
	x[ 948 ] = 948;
	x[ 949 ] = 949;
	x[ 950 ] = 950;
	x[ 951 ] = 951;
	x[ 952 ] = 952;
	x[ 953 ] = 953;
	x[ 954 ] = 954;
	x[ 955 ] = 955;
	x[ 956 ] = 956;
	x[ 957 ] = 957;
	x[ 958 ] = 958;
	x[ 959 ] = 959;
	x[ 960 ] = 960;
	x[ 961 ] = 961;
	x[ 962 ] = 962;
	x[ 963 ] = 963;
	x[ 964 ] = 964;
	x[ 965 ] = 965;
	x[ 966 ] = 966;
	x[ 967 ] = 967;
	x[ 968 ] = 968;
	x[ 969 ] = 969;
	x[ 970 ] = 970;
	x[ 971 ] = 971;
	x[ 972 ] = 972;
	x[ 973 ] = 973;
	x[ 974 ] = 974;
	x[ 975 ] = 975;
	x[ 976 ] = 976;
	x[ 977 ] = 977;
	x[ 978 ] = 978;
	x[ 979 ] = 979;
	x[ 980 ] = 980;
	x[ 981 ] = 981;
	x[ 982 ] = 982;
	x[ 983 ] = 983;
	x[ 984 ] = 984;
	x[ 985 ] = 985;
	x[ 986 ] = 986;
	x[ 987 ] = 987;
	x[ 988 ] = 988;
	x[ 989 ] = 989;
	x[ 990 ] = 990;
	x[ 991 ] = 991;
	x[ 992 ] = 992;
	x[ 993 ] = 993;
	x[ 994 ] = 994;
	x[ 995 ] = 995;
	x[ 996 ] = 996;
	x[ 997 ] = 997;
	x[ 998 ] = 998;
	x[ 999 ] = 999;
	x[ 1000 ] = 1000;

	int left = 0;
	_counter += 1;
	int right = border;
	_counter += 1;
	int midd = 0;
	_counter += 1;
	while(_counter += 0, true) {
		midd = (left + right) / 2, _counter += 3;
		if (_counter += 1, key < x[ midd ]) {right = midd - 1, _counter += 2;    }
		else {if (_counter += 1, key > x[ midd ]) {left = midd + 1, _counter += 2;    }
		      else {break;    }    }
		if (_counter += 1, left > right) {break;    }
	}
}

void _matrixComputing() {
	double a[ 100 ][ 100 ];
	_counter += 0;
	double b[ 100 ][ 100 ];
	_counter += 0;
	double c[ 100 ][ 100 ];
	_counter += 0;
	int i, j;
	_counter += 0;
	for (i = 0, _counter += 1; _counter += 1, i < par; i++, _counter += 1) {for (j = 0, _counter += 1; _counter += 1, j < par; j++, _counter += 1) {
											a[ i ][ j ] = 1, _counter += 1;
											b[ i ][ j ] = 1, _counter += 1;
											c[ i ][ j ] = 0, _counter += 1;
										}    }
	i = 4, _counter += 1;
	j = 0, _counter += 1;
	_counter += 1;
	for (int v = 0; _counter += 1, v < par; v++, _counter += 1) {
		a[ i ][ j ] = 2, _counter += 1;
		i -= 1, _counter += 2;
		j += 1, _counter += 2;
	}
	i = 4, _counter += 1;
	j = 4, _counter += 1;
	_counter += 1;
	for (int v = 0; _counter += 1, v < par; v++, _counter += 1) {
		b[ i ][ j ] = 2, _counter += 1;
		i -= 1, _counter += 2;
		j -= 1, _counter += 2;
	}
	a[ 2 ][ 4 ] = 3, _counter += 1;
	for (i = 0, _counter += 1; _counter += 1, i < par; i++, _counter += 1) {
		for (j = 0, _counter += 1; _counter += 1, j < par; j++, _counter += 1) {cout << "", _counter += 0;    }
	}
	b[ 3 ][ 2 ] = 5, _counter += 1;
	for (i = 0, _counter += 1; _counter += 1, i < par; i++, _counter += 1) {
		for (j = 0, _counter += 1; _counter += 1, j < par; j++, _counter += 1) {cout << "", _counter += 0;    }
	}
	for (i = 0, _counter += 1; _counter += 1, i < par; i++, _counter += 1) {for (j = 0, _counter += 1; _counter += 1, j < par; j++, _counter += 1) {_counter += 1;
											                                                                for (int k = 0; _counter += 1, k < par; k++, _counter += 1) {c[ i ][ j ] += a[ i ][ k ] * b[ k ][ j ], _counter += 3;    }    }    }
	for (i = 0, _counter += 1; _counter += 1, i < par; i++, _counter += 1) {
		for (j = 0, _counter += 1; _counter += 1, j < par; j++, _counter += 1) {cout << "", _counter += 0;    }
	}
}

void _main() {
	int a = 5;


	_matrixComputing(), _counter += 0;
}

void fillComplexityFuncInThread(int thread_number) {
	global_thread_number = thread_number;
	int index = 0;
	for (par = 4 + 1 * global_thread_number; par < 100; par += 1 * 4) {
		_counter = 0;
		_main();
		func_vec[global_thread_number + index] = make_pair(par, _counter);
		index += 4;
	}
}

void createComplexityFunc() {
	thread t0(fillComplexityFuncInThread, 0);
	thread t1(fillComplexityFuncInThread, 1);
	thread t2(fillComplexityFuncInThread, 2);
	thread t3(fillComplexityFuncInThread, 3);
	t0.join();
	t1.join();
	t2.join();
	t3.join();
}

void findComplexityClass() {
	vector<pair<long long, double> > func;
	vector<pair<long long, double> > logarithmic_func;
	vector<pair<long long, double> > exponential_func;
	pair<double, double> params;
	int polynomial_degree, logarithmic_degree, exponential_degree, func_size, part, mod;
	double polynomial_error, logarithmic_error, exponential_error;

	int polynomial_error_increasings = 0;
	int logarithmic_error_increasings = 0;
	int exponential_error_increasings = 0;
	int polynomial_degree_changes = 0;
	int logarithmic_degree_changes = 0;
	int exponential_degree_changes = 0;
	double last_error = INFINITY;
	int last_degree = -1;
	const int PARTS_NUMBER = 10;
	complete_complexity_function = "";

	createComplexityFunc();

	func = func_vec;

	logarithmic_func = func;
	for (pair<long long, double>& element : logarithmic_func) {
		element.second /= log10(element.first);
	}

	exponential_func = func;

	deleteOverflowing(func_vec);
	deleteOverflowing(func);

	func_size = func.size();
	part = func_size / PARTS_NUMBER;
	mod = func_size - part * PARTS_NUMBER;

	for (int i = part + mod; i < func_size; i += part) {
		vector<pair<long long, double> > current_func(func.begin(), func.begin() + i);
		params = linearRegression(current_func, true, true);

		double current_error = params.first;
		int current_degree = round(abs(params.second));

		if (current_error > last_error * 1.1)
			polynomial_error_increasings++;
		last_error = current_error;

		if (current_degree != last_degree)
			polynomial_degree_changes++;
		last_degree = current_degree;
	}

	polynomial_error = last_error;
	polynomial_degree = last_degree;

	last_error = INFINITY;
	last_degree = -1;

	deleteOverflowing(logarithmic_func);

	func_size = logarithmic_func.size();
	part = func_size / PARTS_NUMBER;
	mod = func_size - part * PARTS_NUMBER;

	for (int i = part + mod; i < func_size; i += part) {
		vector<pair<long long, double> > current_func(logarithmic_func.begin(), logarithmic_func.begin() + i);
		params = linearRegression(current_func, true, true);

		double current_error = params.first;
		int current_degree = round(abs(params.second));

		if (current_error > last_error * 1.1)
			logarithmic_error_increasings++;
		last_error = current_error;

		if (current_degree != last_degree)
			logarithmic_degree_changes++;
		last_degree = current_degree;
	}

	logarithmic_error = last_error;
	logarithmic_degree = last_degree;

	last_error = INFINITY;
	last_degree = -1;

	deleteOverflowing(exponential_func);

	func_size = exponential_func.size();
	part = func_size / PARTS_NUMBER;
	mod = func_size - part * PARTS_NUMBER;

	for (int i = part + mod; i < func_size; i += part) {
		vector<pair<long long, double> > current_func(exponential_func.begin(), exponential_func.begin() + i);
		params = linearRegression(current_func, false, true);

		double current_error = params.first;
		int current_degree = round(abs(params.second));

		if (current_error > last_error * 1.1)
			exponential_error_increasings++;
		last_error = current_error;

		if (current_degree != last_degree)
			exponential_degree_changes++;
		last_degree = current_degree;
	}

	exponential_error = last_error;
	exponential_degree = last_degree;

	bool class_is_unknown = false;

	complexity_class = "O(";

	if (polynomial_error < logarithmic_error && polynomial_error < exponential_error) {
		if ((polynomial_error_increasings <= PARTS_NUMBER / 2 || polynomial_error < 1e-04) && polynomial_degree_changes < 5) {
			if (polynomial_degree != 0) {
				complexity_class += "x";
				if (polynomial_degree > 1 && polynomial_degree <= 5)
					complexity_class += "^" + to_string(polynomial_degree);
				else if (polynomial_degree > 5)
					complexity_class = "> " + complexity_class + "^5";
			}
			else
				complexity_class += "1";
			complete_complexity_function = "; " + polynomialComplexityFunction(polynomial_degree, func_vec);
		}
		else
			class_is_unknown = true;
	}
	else if (logarithmic_error < polynomial_error && logarithmic_error < exponential_error) {
		if ((logarithmic_error_increasings <= PARTS_NUMBER / 2 || logarithmic_error < 1e-04) && logarithmic_degree_changes < 5) {
			if (logarithmic_degree != 0) {
				complexity_class += "x";
				if (logarithmic_degree > 1 && logarithmic_degree <= 5)
					complexity_class += "^" + to_string(logarithmic_degree);
				else if (logarithmic_degree > 5)
					complexity_class = "> " + complexity_class + "^5";
				complexity_class += " * ";
			}
			complexity_class += "log(x)";
		}
		else
			class_is_unknown = true;
	}
	else if (exponential_error <= polynomial_error && exponential_error <= logarithmic_error) {
		if ((exponential_error_increasings <= PARTS_NUMBER / 2 || exponential_error < 1e-04) && exponential_degree_changes < 5) {
			if (exponential_degree != 0) {
				complexity_class += "e^";
				if (exponential_degree > 1)
					complexity_class += "(" + to_string(exponential_degree) + "x)";
				else
					complexity_class += "x";
			}
			else
				complexity_class += "1";
		}
		else
			class_is_unknown = true;
	}
	else
		class_is_unknown = true;

	complexity_class += ")" + complete_complexity_function;

	if (class_is_unknown)
		complexity_class = "Unknown";

	w = wstring(complexity_class.begin(), complexity_class.end());
	graph_title = w.c_str();

}
int main() {
	findComplexityClass();

	RGBABitmapImageReference* imageReference = CreateRGBABitmapImageReference();

	for (auto points_pair = func_vec.begin(); points_pair != func_vec.end(); points_pair++) {
		xs.push_back((*points_pair).first);
		ys.push_back((*points_pair).second);
	}

	ScatterPlotSeries* series = GetDefaultScatterPlotSeriesSettings();
	series->xs = &xs;
	series->ys = &ys;
	series->linearInterpolation = true;
	series->lineThickness = 5;
	series->color = CreateRGBColor(0.5, 0.7, 1);

	ScatterPlotSettings* settings = GetDefaultScatterPlotSettings();
	settings->width = 1000;
	settings->height = 1000;
	settings->autoBoundaries = true;
	settings->autoPadding = true;
	settings->title = toVector(graph_title);
	settings->xLabel = toVector(L"complexity parameter");
	settings->yLabel = toVector(L"operations");
	settings->scatterPlotSeries->push_back(series);
	settings->gridColor = CreateRGBColor(0.6, 0.6, 0.6);
	settings->showGrid = true;

	DrawScatterPlotFromSettings(imageReference, settings);

	vector<double>* pngdata = ConvertToPNG(imageReference->image);

	WriteToFile(pngdata, "func.png");
	DeleteImage(imageReference->image);
	system("start func.png");
}