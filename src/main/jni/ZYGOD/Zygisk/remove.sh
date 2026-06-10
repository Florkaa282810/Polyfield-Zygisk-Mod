
for file in *.h; do
    
    sed 's|//.*||' "$file" > temp && mv temp "$file"
done
