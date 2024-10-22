#pragma once

enum class KeyState
{
    Tap,  // �� ���� ����
    Hold, // ������ �ִ�
    Away, // �� �� ����
    None, // ������ �ʾҰ�, �������� ������ ���� ����
};

enum class Key
{
    Left,
    Right,
    Up,
    Down,

    Q,
    W,
    E,
    R,
    T,
    Y,
    U,
    O,
    P,
    A,
    S,
    D,
    F,
    G,
    H,
    J,
    K,
    L,
    Z,
    X,
    C,
    V,
    B,
    N,
    M,

    Alt,
    Ctrl,
    LShift,
    Space,
    Enter,
    ESC,
    Tab,

    LMouse,
    RMouse,
    F1,
    F2,
    F3,
    F4,
    F5,
    F6,
    F7,
    F8,

    Ins,
    Del,
    Home,
    End,
    PgUp,
    PgDn,

    Last, // enum�� ��
};