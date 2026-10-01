from pathlib import Path

def generate_header_list(source_dir, output_file):
    source_path = Path(source_dir)
    unique_headers = set()

    # Recursively scan the directory for all .h files
    for file_path in source_path.rglob("*.h"):
        if file_path.is_file():
            # Use file_path.name to deduplicate by filename (e.g., multiple z3.h instances)
            # If you want unique file paths instead, change this to str(file_path)
            unique_headers.add(file_path.name)

    # Sort alphabetically (case-insensitive)
    sorted_headers = sorted(unique_headers, key=lambda s: s.lower())

    # Write the sorted list to a text file
    with open(output_file, 'w', encoding='utf-8') as f:
        for header in sorted_headers:
            f.write(header + '\n')

    print(f"Successfully wrote {len(sorted_headers)} unique headers to '{output_file}'.")

if __name__ == "__main__":
    # Change "." to your target directory path if it's elsewhere
    DIRECTORY_TO_SCAN = "."
    OUTPUT_TEXT_FILE = "alphabetical_headers.txt"
    
    generate_header_list(DIRECTORY_TO_SCAN, OUTPUT_TEXT_FILE)
