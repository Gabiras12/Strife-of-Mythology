w = 100.0
h = 81.0
x0 = 1024.0 / 2
y0 = h / 2
offset = 11.0

def grid_to_iso(xg, yg):

    xs = (xg - yg) * (w / 2.0) + x0
    ys = (xg + yg) * (h / 2.0 - offset) + y0

    return xs, ys


def iso_to_grid(xs, ys):

    xg = (xs - x0)/w + (ys - y0)/(h - 2.0*offset)
    yg = -(xs - x0)/w + (ys - y0)/(h - 2.0*offset)

    return xg, yg

if __name__ == '__main__':

    xg = float(raw_input())
    yg = float(raw_input())

    print 'xg = %.2f, yg = %.2f' % (xg, yg)

    xs, ys = grid_to_iso(xg, yg)

    print 'xs = %.2f, ys = %.2f' % (xs, ys)

    xn, yn = iso_to_grid(xs, ys)

    print 'xn = %.2f, yn = %.2f' % (xn, yn)
