#!/usr/bin/env -S awk -f
# vim:sw=2:et:

/^Subject:/ {
  start_title = 1;
  title = $0;
  sub(/.?*\] /, "", title);
  next;
}

/^ .+/ {
  if (start_title == 1) {
    title = title $0;
    next;
  }
}

/^Date:/ {
  date = $0;
  sub(/.?*: /, "", date);
  next;
}

/^$/ {
  start_body = 1;
  start_title = 0;
  next;
}

/^[A-Z][a-z-]+:/ {
  if (start_body == 1) {
    next;
  }
}

!/(^---$|^[(]cherry[ -]picked)/ {
  if (start_body == 1) {
    body = body $0;
    next;
  }
}

/(^---$|^[(]cherry[ -]picked)/ {
  print date "|" title "|" body;
  exit;
}
