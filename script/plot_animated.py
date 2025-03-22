#%%
import numpy as np
from matplotlib import animation
from numpy.typing import NDArray
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import matplotlib
from setuptools.sandbox import save_path

# For interactive mode (needed for animations in some environments)
matplotlib.use('TkAgg')  # Use appropriate backend (TkAgg, Qt5Agg, etc.)

class Mesh:
    nlocal : int
    nnodes : int
    nelem : int
    nodes : NDArray[np.float64]
    elem : NDArray[np.int32]
    def __init__(self, fname):
        self.fname = fname
        with open(fname, "r") as f:
            self.nnodes = int(f.readline().split(" ")[3])
            self.nodes = np.zeros((self.nnodes, 2))
            for i in range(self.nnodes):
                line = f.readline()
                parts = [s.strip() for s in line.split(':')]
                self.nodes[i] = [float(val.strip()) for val in parts[1].split()]

            line = f.readline()
            while("triangles" not in line and "quads" not in line):
                line = f.readline()

            self.nlocal = 3 if "triangles" in line else 4
            self.nelem = int(line.split(" ")[3])
            self.elem = np.zeros((self.nelem, self.nlocal), dtype=np.int32)
            for i in range(self.nelem):
                line = f.readline()
                parts = [s.strip() for s in line.split(':')]
                self.elem[i] = [int(val.strip()) for val in parts[1].split()]

    def __str__(self):
        return f"Mesh: {self.fname}\n├─nnodes: {self.nnodes}\n├─nelem: {self.nelem}\n├─local: {self.nlocal}\n├─nodes: array(shape=({self.nodes.shape[0]},{self.nodes.shape[1]}), dtype=np.float64)\n└─elem: array(shape=({self.elem.shape[0]},{self.elem.shape[1]}), dtype=np.int32)\n"

    def __repr__(self) -> str:
        return self.__str__()

    def plot(self, displace=None, *args, **kwargs):
        coord = self.nodes if displace is None else np.array(displace) + self.nodes
        if self.nlocal == 3:
            a = plt.triplot(coord[:,0], coord[:,1], self.elem, *args, **kwargs)
        else:
            y = coord[self.elem[:, [0, 1, 2, 3, 0, 0]], 1]
            y[:,5] = np.nan
            y = y.ravel()
            x = coord[self.elem[:, [0, 1, 2, 3, 0, 0]], 0]
            x[:,5] = np.nan
            x = x.ravel()
            print(x.shape)
            a = plt.plot(x, y, *args, **kwargs)
        return a

    def plotfield(self, field, displace=None, *args, **kwargs):
        coord = self.nodes if displace is None else np.array(displace) + self.nodes
        field = np.array(field)
        if len(field) != self.nnodes and field.size != self.nnodes:
            raise ValueError("Field must be a 1D array with length equal to number of nodes")
        field = field.reshape(-1)
        if self.nlocal == 3:
            a = plt.tripcolor(coord[:,0], coord[:,1], self.elem, field, shading="gouraud", *args, **kwargs)
        else:
            vmin = kwargs.pop("vmin", field.min())
            vmax = kwargs.pop("vmax", field.max())
            for i in range(self.nelem):
                x = coord[self.elem[i, [[0, 1], [3,2]]], 0]
                y = coord[self.elem[i, [[0, 1], [3,2]]], 1]
                f = field[self.elem[i, [[0, 1], [3,2]]]]
                a = plt.pcolormesh(x, y, f, shading="gouraud", linewidth=1, edgecolors="k",  *args, vmin=vmin, vmax=vmax, **kwargs)
        return a

    def number(self, displace=None):
        coord = self.nodes if displace is None else np.array(displace) + self.nodes
        for i, node in enumerate(coord):
            # bg color white
            plt.text(node[0], node[1], str(i), fontsize=8, ha="center", va="center", bbox=dict(facecolor="white", edgecolor="white", boxstyle="circle", pad=0.1, alpha=0.4))
        return self


def animate_mesh_with_field(mesh, displacement, field, frames=60, interval=50, save_path=None,
                            field_kwargs=None, mesh_kwargs=None):
    """
    Animate mesh displacement with field visualization

    Parameters:
    -----------
    mesh : Mesh object
        The mesh to animate
    displacement : ndarray
        Displacement field (should be same shape as mesh.nodes)
    field : ndarray
        Field data to visualize (e.g., uv_norm)
    frames : int
        Number of frames in animation
    interval : int
        Time between frames in milliseconds
    save_path : str, optional
        Path to save animation file (e.g., 'animation.gif')
    field_kwargs : dict, optional
        Additional arguments for plotfield
    mesh_kwargs : dict, optional
        Additional arguments for mesh plot
    """
    # Default arguments if none provided
    if field_kwargs is None:
        field_kwargs = {'cmap': 'turbo'}
    if mesh_kwargs is None:
        mesh_kwargs = {'lw': 0.2, 'c': 'k'}

    # Create initial figure to determine proper limits
    fig_temp, ax_temp = plt.subplots(figsize=(10, 6))
    cb = mesh.plotfield(field, displacement, **field_kwargs)
    mesh.plot(displacement, **mesh_kwargs)
    xlim = ax_temp.get_xlim()
    ylim = ax_temp.get_ylim()
    plt.close(fig_temp)  # Close temporary figure

    # Create figure for animation
    fig, ax = plt.subplots(figsize=(10, 6))

    # Add colorbar
    mappable = plt.cm.ScalarMappable(cmap=field_kwargs.get('cmap', 'turbo'))
    mappable.set_array(field)
    cbar = fig.colorbar(mappable, ax=ax)
    cbar.set_label('Displacement magnitude')

    # Function to update plot for each frame
    def update(frame):
        ax.clear()
        factor = frame / (frames - 1)

        # Calculate interpolated displacement
        interp_displace = displacement * factor

        # Plot field visualization
        cb = mesh.plotfield(field, interp_displace, **field_kwargs)

        # Plot mesh lines
        mesh.plot(interp_displace, **mesh_kwargs)

        # Keep consistent axes
        ax.set_xlim(xlim)
        ax.set_ylim(ylim)
        ax.set_aspect('equal')
        ax.grid(alpha=0.2)
        ax.set_title(f'Displacement Scale: {factor:.2f}')

        return [ax]

    # Create animation
    anim = FuncAnimation(fig, update, frames=frames, interval=interval, blit=False)

    # Save if path provided
    if save_path:
        writer = animation.FFMpegWriter(fps=30, extra_args=["-vcodec", "libx264", "-preset", "slow", "-crf", "18"])
        anim.save("anim.mp4", writer=writer, dpi=300)

    return anim, fig


if __name__ == "__main__":
    mesh = Mesh("data/mesh.txt")
    print(mesh)
    uv = np.loadtxt("data/UV.txt", skiprows=1, delimiter=",")
    uv_norm = np.linalg.norm(uv, axis=1)
    factor = 5e4

    # Create displacement field
    displacement = uv * factor

    # Create and display animation with field visualization
    anim, fig = animate_mesh_with_field(
        mesh,
        displacement,
        uv_norm,
        frames=60,
        interval=50,
        field_kwargs={'cmap': 'turbo'},
        mesh_kwargs={'lw': 0.2, 'c': 'k'},
        save_path="animation.mp4"
    )

    plt.show()

    # Optional: save animation
    # anim.save('mesh_displacement_with_field.gif', writer='pillow')