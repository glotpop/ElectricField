// ElectricField.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
#include <Windows.h>
#include "resource.h"


#include "vec2.h"



int height, width;

using Sources = std::vector<std::pair<double, vec2>>;

const double ONE = 1;

const Sources sources = { {1, vec2(-ONE, ONE)}, {-1, vec2(ONE, ONE)},
                          {-1, vec2(-ONE, -ONE)}, {1, vec2(ONE, -ONE)}
                        };

const double SCALE = 2;
std::vector<std::pair<int, int>> coords = {};


const int clamp(int val, int lo, int hi) {
    if (val < lo) {
        return lo;
    }
    if (val > hi) {
        return hi;
    }
    return val;
}


const vec2 calcElectricalIntensity(const vec2& dst, const Sources& sources) {
    auto result = vec2(0, 0);
    for (const auto& src : sources) {
        const auto r2 = (dst - src.second).length2();
        result += (dst - src.second).normalized() * (src.first / r2);
    }
    return result;
}

const vec2 calcFieldForce(const vec2& dst, double q, const Sources& sources) {
    return calcElectricalIntensity(dst, sources) * q;
}

const vec2 toImCoords(int x, int y, int width, int height, double scale) {
    double x_ = x - width / 2;
    double y_ = height / 2 - y;
    return vec2(x_, y_) * scale;
}

const std::pair<int, int> toRealCoords(const vec2& im, int width, int height, double scale) {
    const auto norm = im / scale;
    return std::make_pair(norm.x() + width / 2, height / 2 - norm.y());
}


int WINAPI DlgProc(HWND hDlg, WORD wMsg, WORD wParam, DWORD) {
    PAINTSTRUCT ps;
    if (wMsg == WM_CLOSE || wMsg == WM_COMMAND && wParam == IDOK) {
        EndDialog(hDlg, 0);
    }
    if (wMsg == WM_INITDIALOG) {
        RECT rc;
        GetClientRect(hDlg, &rc);
        width = rc.right - rc.left;
        height = rc.bottom - rc.top;
    }
    else if (wMsg == WM_PAINT) {
        BeginPaint(hDlg, &ps);
        HPEN hPen = (HPEN)CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
        HPEN hOldPen = (HPEN)SelectObject(ps.hdc, hPen);

        double q = 1e7;

        int qX = width / 10;
        int qY = height / 2;

        auto start = toImCoords(qX, qY, width, height, SCALE);
        auto acc = calcFieldForce(start, q, sources);
        auto oldAcc = acc;

        auto velocity = vec2(0, 0);
        auto oldVelocity = velocity;
        const auto dt = 0.1;

        while (0 < qX && qX < width && 0 < qY && qY < height) {
            acc = calcFieldForce(start, q, sources);

            velocity += (acc + oldAcc) * dt / 2;
            start += (velocity + oldVelocity) * dt / 2;

            oldVelocity = velocity;
            oldAcc = acc;
            const auto end = toRealCoords(start, width, height, SCALE);
            qX = clamp(end.first, 0, width);
            qY = clamp(end.second, 0, height);
            //coords.push_back(std::make_pair(qX, qY));
            Rectangle(ps.hdc, qX - 1, qY - 1, qX + 1, qY + 1);
            std::cout << qX << ' ' << qY << std::endl;
        }

        //for (const auto v : coords) {
        //    int qX = v.first;
        //    int qY = v.second;
        //    Rectangle(ps.hdc, qX - 1, qY - 1, qX + 1, qY + 1);
        //}

        //for (int i = 0; i < width; i+=20) {
        //    for (int j = 0; j < height; j+=20) {
        //        const auto start = toImCoords(i, j, width, height, SCALE);
        //        auto elIntensity = calcElectricalIntensity(start, sources).normalized() * (SCALE * 10);
        //        const auto end = toRealCoords(start + elIntensity, width, height, SCALE);

        //        const auto endX = clamp(end.first, 0, width);
        //        const auto endY = clamp(end.second, 0, height);

        //        MoveToEx(ps.hdc, i, j, NULL);
        //        LineTo(ps.hdc, endX, endY);
        //        

        //        // int x = static_cast<double>(i) / static_cast<double>(width) * 255;
        //        // int y = static_cast<double>(j) / static_cast<double>(height) * 255;
        //        // int z = static_cast<double>((i + 1) * (j + 1)) / static_cast<double>(width * height) * 255;
        //        // SetPixel(ps.hdc, i, j, RGB(x, y, z));
        //    }
        //}
        SelectObject(ps.hdc, hOldPen);
        DeleteObject(hPen);
        EndPaint(hDlg, &ps);
    }
    return 0;
}


int main()
{

    DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC) DlgProc);
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
