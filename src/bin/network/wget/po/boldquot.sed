# For quotearg:
s/^`$/โ[1m/
s/^'$/โ[0m/

s/"\([^"]*\)"/โ\1โ/g
s/`\([^`']*\)'/โ\1โ/g
s/ '\([^`']*\)' / โ\1โ /g
s/ '\([^`']*\)'$/ โ\1โ/g
s/^'\([^`']*\)' /โ\1โ /g
s/โโ/""/g
s/โ/โ[1m/g
s/โ/[0mโ/g
s/โ/โ[1m/g
s/โ/[0mโ/g

# At least in all of our current strings, ' should be โ.
s/'/โ/g
# Special: write Hrvojeโs last name properly.
s/Niksic/Nikลกiฤ/g
s/opyright (C)/opyright ยฉ/g
