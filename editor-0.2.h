#include <ncurses.h>
#include <bits/stdc++.h>
#include <fstream>
class texteditor
{
private:
    std::string filebuf;
    int maxln;
    std::vector<std::string> buf;
    void open()
    {
        if (!std::filesystem::exists(filebuf))
        {
            std::ofstream createfile(filebuf);
            createfile.close();
            return;
        }
        else
        {
            std::fstream readfile(filebuf);
            std::string b;
            while (!readfile.eof())
            {
                std::getline(readfile, b);
                buf.push_back(b);
            }
            readfile.close();
            return;
        }
    }
    void edit()
    {
        std::string back = "";
        int y, x, m = 0, R, F, N, P, C, maxbuf = 1024 * 1024;
        {
            set_tabsize(8);
            std::vector<std::string> temp(maxbuf);
            wdisplay(m);
            transfer(temp);
            while (true)
            {
                int ch = getch();
                if (ch == 19)
                    break;
                getyx(stdscr, y, x);
                switch (ch)
                {
                case (KEY_LEFT):
                    if (y == 0 && x == 0)
                        break;
                    else if (x == 0)
                    {
                        y--;
                        move(y, x = temp[y].size());
                    }
                    else
                    {
                        move(y, x -= 1);
                    }
                    break;
                case (KEY_RIGHT):
                    R = temp[y].size();
                    if (y == m && x == R)
                        break;
                    else if (x == R)
                        move(y += 1, 0);
                    else
                    {
                        move(y, x += 1);
                    }
                    break;
                case (KEY_UP):
                    if (y == 0)
                        break;
                    else
                    {
                        P = temp[y - 1].size();
                    }
                    if (P < x)
                    {
                        y--;
                        move(y, x = P);
                    }
                    else
                    {
                        move(y -= 1, x);
                    }
                    break;
                case (KEY_DOWN):
                    N = temp[y + 1].size();
                    if (y == m)
                        break;
                    else if (x > N)
                        move(y += 1, x = N);
                    else
                    {
                        move(y += 1, x);
                    }
                    break;
                case (KEY_BACKSPACE):
                    if (y == 0 && x == 0)
                        break;
                    else if (x == 0)
                    {
                        back = temp[y];
                        temp.erase(temp.begin() + y);
                        deleteln();
                        m--, y--;
                        C = temp[y].size();
                        move(y, x = C);
                        insstr(back.c_str());
                        temp[y] += back;
                        back = "";
                    }
                    else
                    {
                        temp[y].erase(temp[y].begin() + x -= 1);
                        mvdelch(y, x -= 1);
                    }
                    break;
                case (10):
                    if (x == 0)
                    {
                        temp.emplace(temp.begin() + y, "");
                        insertln();
                        move(y += 1, 0);
                    }
                    else if (x == temp[y].size())
                    {
                        y++;
                        temp.emplace(temp.begin() + y, "");
                        move(y, 0);
                        insertln();
                    }
                    else
                    {
                        back = temp[y].substr(x);
                        clrtoeol();
                        temp[y].erase(temp[y].begin() + x, temp[y].end());
                        y++;
                        temp.emplace(temp.begin() + y, back);
                        move(y, x = 0);
                        insertln();
                        insstr(back.c_str());
                    }
                    m++;
                    break;
                case (9):
                    for(int i =0;i<8;i++){
                        temp[y].insert(temp[y].begin() + x,' ');    
                        insch(' ');
                    }
                    move(y,x+=8);
                    break;
                default:
                    temp[y].insert(temp[y].begin() + x, char(ch));
                    insch(ch);
                    move(y, x += 1);
                    break;
                }
                refresh();
            }
            maxln = m;
            temptobuf(temp);
        }
    }
    void wdisplay(int &m)
    {
        if (!buf.empty())
        {
            for (auto it : buf)
            {
                printw("%s\n", it.c_str());
                m++;
            }
        }
        refresh();
    }
    void transfer(std::vector<std::string> &t)
    {
        int i = 0;
        for (auto it : buf)
        {
            t[i] = it;
            i++;
        }
        buf.clear();
    }
    void temptobuf(std::vector<std::string> &t)
    {
        for (int i = 0; i <= maxln; i++)
        {
            buf.push_back(t[i]);
        }
        t.clear();
    }
    void save()
    {
        std::ofstream readfile(filebuf);
        for (auto b : buf)
        {
            readfile << b + "\n";
        }
        readfile.close();
    }

public:
    texteditor(std::string file) : filebuf(file)
    {
        open();
        initscr();
        cbreak();
        raw();
        noecho();
        scrollok(stdscr, true);
        keypad(stdscr, true);
        refresh();
        edit();
        save();
    }
    ~texteditor()
    {
        endwin();
        std::cout << "Your File is Saved:" << filebuf << std::endl;
    }
};