#!/usr/bin/env -S awk -f
# vim:sw=2:et:

/^Date:/ {
  date = $0;
  sub(/^Date: /, "", date);
  next;
}

/^Subject:/ {
  title = $0;
  sub(/.?*\] /, "", title);
  next;
}

/^ .+/ {
  if (title != "") {
    title = title $0;
  }
  next;
}

{
  if (title) {
    print date, title;
    exit;
  }
}
