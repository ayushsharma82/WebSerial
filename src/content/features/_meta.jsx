import Badge from "@/src/components/Badge";

const metadata = {
  callbacks: {
    title: "Callbacks"
  },
  printing: {
    title: "Printing"
  },
  timestamps: {
    title: <span className="flex items-center justify-center gap-2">Timestamps <Badge /></span>,
  },
  font: {
    title: <span className="flex items-center justify-center gap-2">Font Customization <Badge /></span>,
  },
  export: {
    title: <span className="flex items-center justify-center gap-2">Export Logs <Badge /></span>,
  },
  "hardware-id": {
    title: <span className="flex items-center justify-center gap-2">Hardware ID <Badge /></span>,
  },
  "custom-title": {
    title: <span className="flex items-center justify-center gap-2">Custom Title <Badge /></span>,
  }
}

export default metadata