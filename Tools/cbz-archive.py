import os
from datetime import datetime
import tkinter.filedialog
import tkinter.messagebox
import tkinter.ttk
import zipfile
import tkinter
import re
import threading

APP_NAME = "CBZ Archiver"
DEFAULT_IGNORE = ".nomedia"
IGNORE_DESC = "Ignore filenames separated with comma(,), can include regex"

def log(v: str):
    now = datetime.now()
    now = now.strftime(r"%Y-%m-%d %H:%M.%S")
    print(f"[{now}] {v}")

def invoke_path_picker(entry: tkinter.Entry):
    path = tkinter.filedialog.askdirectory(title="Select a folder")
    entry.delete(0, tkinter.END)
    entry.insert(0, path)

g_label_progress_bar: tkinter.Label | None = None
g_progress_bar: tkinter.ttk.Progressbar | None = None
g_entry_input_dir: tkinter.Entry | None = None
g_entry_output_dir: tkinter.Entry | None = None
g_entry_ignore_name: tkinter.Entry | None = None
g_btn_archive: tkinter.Button | None = None

g_errs_paths = []
g_count_total_files = 0
g_count_archive_done = 0

class ThreadHandler:
    lock = threading.Lock()
    threads = [None for _ in range(4)]
    th_queue = []
    on_stop = None

    def process(index, action):
        action()

        with ThreadHandler.lock:
            ThreadHandler.threads[index] = None

            if len(ThreadHandler.th_queue) > 0:
                ThreadHandler.threads[index] = threading.Thread(
                    target=ThreadHandler.process,
                    args=(index, ThreadHandler.th_queue.pop(0), )
                )
                ThreadHandler.threads[index].start()

            ThreadHandler.on_thread_stop()

    def push(action):
        with ThreadHandler.lock:
            for i in range(len(ThreadHandler.threads)):
                if ThreadHandler.threads[i] != None:
                    continue
                if len(ThreadHandler.th_queue) == 0:
                    ThreadHandler.threads[i] = threading.Thread(target=ThreadHandler.process, args=(i, action, ))
                    ThreadHandler.threads[i].start()
                    return
                else:
                    queued_action = ThreadHandler.th_queue.pop(0)
                    ThreadHandler.threads[i] = threading.Thread(target=ThreadHandler.process, args=(i, queued_action, ))
                    ThreadHandler.threads[i].start()
                    ThreadHandler.th_queue.append(action)
                    return
            ThreadHandler.th_queue.append(action)

    def on_thread_stop():
        for i in range(len(ThreadHandler.threads)):
            if ThreadHandler.threads[i] != None:
                return
        
        if len(ThreadHandler.th_queue) > 0:
            return

        assert(ThreadHandler.on_stop != None, "Set on_stop")
        ThreadHandler.on_stop()

def on_archiving_done():
    global g_progress_bar, g_count_total_files, g_errs_paths, g_count_archive_done, g_btn_archive, g_label_progress_bar
    g_progress_bar["value"] = 0

    g_btn_archive["state"] = tkinter.NORMAL
    g_count_total_files = 0
    g_count_archive_done = 0

    if len(g_errs_paths) > 0:
        err = "\n".join(g_errs_paths)
        tkinter.messagebox.showerror("Error", f"{err}")
    
    g_label_progress_bar.config(text="Done")
    g_errs_paths = []

def invoke_archive():
    global g_progress_bar, g_btn_archive

    assert(g_progress_bar != None and g_entry_input_dir != None and g_entry_output_dir != None)
    input_dir = g_entry_input_dir.get()
    output_dir = g_entry_output_dir.get()
    ignore_names = g_entry_ignore_name.get().split(',')

    if len(input_dir) == 0 or not os.path.exists(input_dir) or not os.path.isdir(input_dir):
        tkinter.messagebox.showerror("Error", f"{input_dir} does not exist or is not a directory")
        return

    if len(output_dir) == 0 or not os.path.exists(output_dir) or not os.path.isdir(output_dir):
        tkinter.messagebox.showerror("Error", f"{output_dir} does not exist or is not a directory")
        return

    g_progress_bar["value"] = 0
    g_btn_archive["state"] = tkinter.DISABLED
    ThreadHandler.on_stop = on_archiving_done
    ThreadHandler.push(lambda i=input_dir, o=output_dir, ig=ignore_names: archive(i,o,ig))

def update_progress_bar(text):
    global g_progress_bar, g_label_progress_bar
    if g_count_total_files == 0 or g_count_archive_done == 0:
        return

    done = g_count_archive_done + len(g_errs_paths)
    ratio = 0
    if done > g_count_total_files:
        ratio = 100
    else:
        ratio = done / g_count_total_files * 100
    g_progress_bar["value"] = max(0, min(ratio, 100))
    assert(g_label_progress_bar != None)
    g_label_progress_bar.config(text=text)

def archive(current_path: str, out_path: str, ignore: list[str]):
    global g_errs_paths, g_count_archive_done, g_count_total_files
    basename_current_path = os.path.basename(current_path)

    names = os.listdir(current_path)
    if len(names) == 0:
        return

    log(f"working on '{current_path}'")
    files = []
    dirs = []
    for name in names:
        full_path = os.path.join(current_path, name)
        basename = os.path.basename(name)
        cont = False
        for ig in ignore:
            if re.match(ig, basename) != None:
                cont = True
                break

        if cont:
            continue

        if os.path.isdir(full_path):
            dirs.append(full_path)
        else:
            files.append(full_path)

    try:
        if len(files) > 0:
            basename_current_path_cbz = os.path.join(out_path, basename_current_path + ".cbz")
            with ThreadHandler.lock:
                g_count_total_files += len(files)
            with zipfile.ZipFile(basename_current_path_cbz, "w", zipfile.ZIP_STORED) as z:
                for file in files:
                    basename = os.path.basename(file)
                    z.write(file, basename)
                    with ThreadHandler.lock:
                        g_count_archive_done += 1
                        update_progress_bar(file)
        if len(dirs) > 0:
            out_path = os.path.join(out_path, basename_current_path)
            if not os.path.exists(out_path):
                os.makedirs(out_path)
    except Exception as e:
        with ThreadHandler.lock:
            g_errs_paths.append(f"{current_path} {e}")
    
    for dir in dirs:
        ThreadHandler.push(lambda d=dir, o=out_path, i=ignore: archive(d, o, i))

def main():
    global g_progress_bar, g_entry_input_dir, g_entry_output_dir, g_btn_archive, g_entry_ignore_name, g_label_progress_bar
    tk_root = tkinter.Tk()
    current_path = os.path.abspath(".")
    tk_root.title(APP_NAME)

    entry_frame = tkinter.Frame(tk_root)
    entry_frame.grid(row=0,column=0,padx=5,pady=5, sticky="ew")
    entry_frame.columnconfigure(1, weight=1)

    # Entry Frame
    label_input_dir = tkinter.Label(entry_frame, text ="Source Directory:")
    label_output_dir = tkinter.Label(entry_frame, text = "Output Directory:")
    label_ignore_name = tkinter.Label(entry_frame, text = "Ignore:")
    label_ignore_desc = tkinter.Label(entry_frame, text = IGNORE_DESC)
    label_input_dir.grid(row=0,column=0)
    label_output_dir.grid(row=1,column=0)
    label_ignore_name.grid(row=2,column=0, sticky="e")
    label_ignore_desc.grid(row=3,column=0, sticky="w", padx=5, columnspan=2)
    # tooltip.bind_widget(label_ignore_name, ballonmsg="Ignore filenames separated with comma(,), can include regex")

    g_entry_input_dir = tkinter.Entry(entry_frame)
    g_entry_output_dir = tkinter.Entry(entry_frame)
    g_entry_ignore_name = tkinter.Entry(entry_frame)
    g_entry_output_dir.insert(0, current_path)
    g_entry_ignore_name.insert(0, DEFAULT_IGNORE)

    g_entry_input_dir.grid(row=0,column=1, padx=8, sticky="ew")
    g_entry_output_dir.grid(row=1,column=1, padx=8, sticky="ew")
    g_entry_ignore_name.grid(row=2,column=1, padx=8, sticky="ew")

    btn_input_dir = tkinter.Button(entry_frame, text="Open", command=lambda: invoke_path_picker(g_entry_input_dir))
    btn_output_dir = tkinter.Button(entry_frame, text="Open", command=lambda: invoke_path_picker(g_entry_output_dir))

    btn_input_dir.grid(row=0,column=2, padx=8, sticky="e")
    btn_output_dir.grid(row=1,column=2, padx=8, sticky="e")
    # ...Entry Frame

    g_btn_archive = tkinter.Button(tk_root, text="Archive", command=invoke_archive)
    g_btn_archive.grid(row=1, column=0, sticky="ew", padx=5, pady=5)

    # Progress Bar Frame
    frame_progress_bar = tkinter.Frame(tk_root)
    frame_progress_bar.grid(row=2, column=0, sticky="ew", padx=5, pady=5)
    frame_progress_bar.columnconfigure(0, weight=1)
    g_progress_bar = tkinter.ttk.Progressbar(frame_progress_bar)
    g_progress_bar.grid(row=0, column=0, sticky="ew")
    # progress_bar["value"] = 10

    g_label_progress_bar = tkinter.Label(frame_progress_bar, text="", wraplength=400)
    g_label_progress_bar.grid(row=1,column=0, sticky="w")
    # ...Progress Bar Frame

    tk_root.columnconfigure(0, weight=1)
    tk_root.mainloop()

if __name__ == "__main__":
    main()